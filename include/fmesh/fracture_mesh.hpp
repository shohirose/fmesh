// MIT License
//
// Copyright (c) 2019 Sho Hirose (sho.hirose@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <vector>
#include <algorithm>
#include "fmesh/edge.hpp"
#include "fmesh/index.hpp"
#include "fmesh/iterator_range.hpp"
#include "fmesh/property_array.hpp"

namespace fmesh {

template <typename Point, typename Face>
class fracture_mesh {
 public:
  auto num_vertices() const noexcept { return vertices_.size(); }
  auto num_edges() const noexcept { return edges_.size(); }
  auto num_faces() const noexcept { return faces_.size(); }

  edge_index find(const fmesh::edge& e) const noexcept {
    const auto& es = vertex_edges_[e.first];
    const auto it = std::find_if(es.begin(), es.end(), [e, this](auto ei) {
      return this->edges_[ei] == e;
    });
    return it == es.end() ? edge_index{} : *it;
  }

  face_index find(const Face& f) const noexcept {
    const auto& fs = vertex_faces_[f[0]];
    const auto it = std::find_if(fs.begin(), fs.end(), [&f, this](auto fi) {
      return this->faces_[fi] == f;
    });
    return it == fs.end() ? face_index{} : *it;
  }

  vertex_index add_vertex(const Point& p) {
    const vertex_index vi{vertices_.size()};
    vertices_.push_back(p);
    const auto n = vertices_.size();
    vertex_vertices_.resize(n);
    vertex_edges_.resize(n);
    vertex_faces_.resize(n);
    return vi;
  }

  edge_index add_edge(const fmesh::edge& e) {
    if (this->find(e).is_valid()) {
      std::cerr << "Warning: edge [" << e << "] is already registered.\n";
      return edge_index{};
    }

    const edge_index ei{edges_.size()};
    edges_.push_back(e);
    edge_faces_.resize(edges_.size());
    this->update_edge_connectivity(e, ei);
    return ei;
  }

  face_index add_face(const Face& f) {
    if (this->find(f).is_valid()) {
      std::cerr << "Warning: face [" << f << "] is already registered.\n";
      return face_index{};
    }

    const face_index fi{faces_.size()};
    faces_.push_back(f);
    face_edges_.resize(faces_.size());
    this->update_face_connectivity(f, fi);
    return fi;
  }

  auto vertex_begin() noexcept { return vertices_.begin(); }
  auto vertex_begin() const noexcept { return vertices_.begin(); }
  auto vertex_end() noexcept { return vertices_.end(); }
  auto vertex_end() const noexcept { return vertices_.end(); }

  auto edge_begin() noexcept { return edges_.begin(); }
  auto edge_begin() const noexcept { return edges_.begin(); }
  auto edge_end() noexcept { return edges_.end(); }
  auto edge_end() const noexcept { return edges_.end(); }

  auto face_begin() noexcept { return faces_.begin(); }
  auto face_begin() const noexcept { return faces_.begin(); }
  auto face_end() noexcept { return faces_.end(); }
  auto face_end() const noexcept { return faces_.end(); }

  auto vertices() noexcept {
    return make_iterator_range(this->vertex_begin(), this->vertex_end());
  }
  auto vertices() const noexcept {
    return make_iterator_range(this->vertex_begin(), this->vertex_end());
  }

  auto edges() noexcept {
    return make_iterator_range(this->edge_begin(), this->edge_end());
  }
  auto edges() const noexcept {
    return make_iterator_range(this->edge_begin(), this->edge_end());
  }

  auto faces() noexcept {
    return make_iterator_range(this->face_begin(), this->face_end());
  }
  auto faces() const noexcept {
    return make_iterator_range(this->face_begin(), this->face_end());
  }

  auto& vertex(vertex_index i) noexcept { return vertices_[i]; }
  const auto& vertex(vertex_index i) const noexcept { return vertices_[i]; }
  const auto& edge(edge_index i) const noexcept { return edges_[i]; }
  const auto& face(face_index i) const noexcept { return faces_[i]; }

 private:
  void update_face_connectivity(const Face& f, const face_index fi) noexcept {
    for (auto&& vi : f) vertex_faces_[vi].push_back(fi);
    const auto fedges = f.to_edges();

    for (auto&& e : fedges) {
      const auto ei = this->find(e);

      if (ei.is_valid()) {
        // Edge already resitered
        // So only edge-face connectivity data are updated
        face_edges_[fi].push_back(ei);
        edge_faces_[ei].push_back(fi);
      } else {
        // Edge is new
        // So first resiter the new edge, and then update all connectivity data.
        const edge_index ej{edges_.size()};
        edges_.push_back(e);
        edge_faces_.resize(edges_.size());

        edge_faces_[ej].push_back(fi);
        face_edges_[fi].push_back(ej);
        vertex_edges_[e.first].push_back(ej);
        vertex_edges_[e.second].push_back(ej);
        vertex_vertices_[e.first].push_back(e.second);
        vertex_vertices_[e.second].push_back(e.first);
      }
    }
  }

  void update_edge_connectivity(const fmesh::edge& e,
                                const edge_index ei) noexcept {
    // Update vertex-edges and vertex-vertices
    vertex_edges_[e.first].push_back(ei);
    vertex_edges_[e.second].push_back(ei);
    vertex_vertices_[e.first].push_back(e.second);
    vertex_vertices_[e.second].push_back(e.first);

    // Find faces connected to the edge
    // Faces connected to both ends of the edge are the ones
    const auto& fs1 = vertex_faces_[e.first];
    const auto& fs2 = vertex_faces_[e.second];
    for (auto&& fi : fs1) {
      if (std::find(fs2.begin(), fs2.end(), fi) != fs2.end()) {
        edge_faces_[ei].push_back(fi);
        face_edges_[fi].push_back(ei);
      }
    }
  }

  vertex_property<Point> vertices_;
  edge_property<fmesh::edge> edges_;
  face_property<Face> faces_;
  vertex_property<std::vector<vertex_index>> vertex_vertices_;
  vertex_property<std::vector<edge_index>> vertex_edges_;
  vertex_property<std::vector<face_index>> vertex_faces_;
  edge_property<std::vector<face_index>> edge_faces_;
  face_property<std::vector<edge_index>> face_edges_;
};

}  // namespace fmesh