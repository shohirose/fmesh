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

#ifndef FMESH_FRACTURE_MESH_HPP
#define FMESH_FRACTURE_MESH_HPP

#include <algorithm>
#include <vector>
#include "fmesh/edge.hpp"
#include "fmesh/index.hpp"
#include "fmesh/index_iterator.hpp"
#include "fmesh/iterator_range.hpp"
#include "fmesh/property_array.hpp"

namespace fmesh {

template <typename Point, typename Face>
class fracture_mesh {
 public:
  /// @brief Returns the number of vertices
  auto num_vertices() const noexcept { return vertices_.size(); }

  /// @brief Returns the number of edges
  auto num_edges() const noexcept { return edges_.size(); }

  /// @brief Returns the number of faces
  auto num_faces() const noexcept { return faces_.size(); }

  /// @brief Checks if a vertex is valid
  /// @param[in] i Vertex index
  /// @return Is a given vertex valid?
  bool is_valid(vertex_index i) const noexcept { return is_valid_vertex_[i]; }

  /// @brief Checks if an edge is valid
  /// @param[in] i Edge index
  /// @return Is a given edge valid?
  bool is_valid(edge_index i) const noexcept { return is_valid_edge_[i]; }

  /// @brief Checks if a face is valid
  /// @param[in] i Face index
  /// @return Is a given face valid?
  bool is_valid(face_index i) const noexcept { return is_valid_face_[i]; }

  /// @brief Find the edge index of a given edge
  /// @param[in] e Edge
  /// @return The index of a given edge. If the edge is not found, an invalid
  /// index is returned.
  edge_index find(const fmesh::edge& e) const noexcept {
    const auto& es = vertex_edges_[e.first];
    const auto it = std::find_if(es.begin(), es.end(), [e, this](auto ei) {
      return this->edges_[ei] == e;
    });
    return it == es.end() ? edge_index{} : *it;
  }

  /// @brief Find the face index of a given face
  /// @param[in] f Face
  /// @return The index of a given face. If the face is not found, an invalid
  /// index is returned.s
  face_index find(const Face& f) const noexcept {
    const auto& fs = vertex_faces_[f[0]];
    const auto it = std::find_if(fs.begin(), fs.end(), [&f, this](auto fi) {
      return this->faces_[fi] == f;
    });
    return it == fs.end() ? face_index{} : *it;
  }

  /// @brief Add a new vertex to mesh
  /// @param[in] p A new vertex as a vector
  /// @return The index of a new vertex
  ///
  /// This function updates vertex connectivity info.
  vertex_index add(const Point& p) {
    const vertex_index vi{vertices_.size()};
    vertices_.push_back(p);
    const auto n = vertices_.size();
    vertex_vertices_.resize(n);
    vertex_edges_.resize(n);
    vertex_faces_.resize(n);
    is_valid_vertex_.push_back(true);
    return vi;
  }

  /// @brief Add a new edge to mesh
  /// @param[in] e A new edge
  /// @return The index of a new edge
  ///
  /// This function updates edge connectivity info.
  /// If the given edge is already registered, an invalid edge index will be
  /// returned.
  edge_index add(const fmesh::edge& e) {
    if (this->find(e).is_valid()) {
      std::cerr << "Warning: edge [" << e << "] is already registered.\n";
      return edge_index{};
    }

    const edge_index ei{edges_.size()};
    edges_.push_back(e);
    edge_faces_.resize(edges_.size());
    is_valid_edge_.push_back(true);
    this->update_edge_connectivity(e, ei);
    return ei;
  }

  /// @brief Add a new face to mesh
  /// @param[in] f A new face
  /// @return The index of a new face
  ///
  /// This function updates vertex, edge, and face connectivity info.
  /// If the given face is already registered, an invalid face index will be
  /// returned.
  face_index add(const Face& f) {
    if (this->find(f).is_valid()) {
      std::cerr << "Warning: face [" << f << "] is already registered.\n";
      return face_index{};
    }

    const face_index fi{faces_.size()};
    faces_.push_back(f);
    face_edges_.resize(faces_.size());
    is_valid_face_.push_back(true);
    this->update_face_connectivity(f, fi);
    return fi;
  }

  /// @brief Invalidate a given vertex
  /// @param[in] vi Vertex index to be invalidated
  ///
  /// This function invalidates a given vertex. In addition, it also invalidates
  /// edges and faces connected to the vertex.
  void invalidate(vertex_index vi) {
    has_invalid_vertices_ = true;
    is_valid_vertex_[vi] = false;

    // Invalidate all edges connected to the vertex
    const auto& es = vertex_edges_[vi];
    for (auto&& ei : es) is_valid_edge_[ei] = false;

    // Invalidate all faces connected to the vertex
    const auto& fs = vertex_faces_[vi];
    for (auto&& fi : fs) is_valid_face_[fi] = false;
  }

  /// @brief Invalidate a given edge
  /// @param[in] ei Edge index to be invalidated
  ///
  /// This function invalidates a given edge. In addition, it also invalidates
  /// faces which the edge consists of.
  void invalidate(edge_index ei) {
    has_invalid_edges_ = true;
    is_valid_edge_[ei] = false;

    // Invalidate all faces which the edge consists of
    const auto& fs = edge_faces_[ei];
    for (auto&& fi : fs) is_valid_face_[fi] = false;
  }

  /// @brief Invalidate a given face
  /// @param[in] fi Face index to be invalidated
  void invalidate(face_index fi) {
    has_invalid_faces_ = true;
    is_valid_face_[fi] = false;
  }

  /// @name Vertex iterators
  /// @{
  auto vertex_begin() const noexcept {
    return vertex_iterator{vertex_index{0}};
  }
  auto vertex_end() const noexcept {
    return vertex_iterator{vertex_index{vertices_.size()}};
  }
  /// @}

  /// @name Edge iterators
  /// @{
  auto edge_begin() const noexcept { return edge_iterator{edge_index{0}}; }
  auto edge_end() const noexcept {
    return edge_iterator{edge_index{edges_.size()}};
  }
  /// @}

  /// @name Face iterators
  /// @{
  auto face_begin() const noexcept { return face_iterator{face_index{0}}; }
  auto face_end() const noexcept {
    return face_iterator{face_index{faces_.size()}};
  }
  /// @}

  /// @name Vertex ranges
  /// @{
  auto vertices() noexcept {
    return make_iterator_range(this->vertex_begin(), this->vertex_end());
  }
  auto vertices() const noexcept {
    return make_iterator_range(this->vertex_begin(), this->vertex_end());
  }
  /// @}

  /// @name Edge ranges
  /// @{
  auto edges() noexcept {
    return make_iterator_range(this->edge_begin(), this->edge_end());
  }
  auto edges() const noexcept {
    return make_iterator_range(this->edge_begin(), this->edge_end());
  }
  /// @}

  /// @name Face ranges
  /// @{
  auto faces() noexcept {
    return make_iterator_range(this->face_begin(), this->face_end());
  }
  auto faces() const noexcept {
    return make_iterator_range(this->face_begin(), this->face_end());
  }
  /// @}

  /// @name Accessors
  /// @{

  /// @brief Get reference to vertex
  /// @param[in] i Vertex index
  /// @return Reference to vertex
  auto& vertex(vertex_index i) noexcept { return vertices_[i]; }

  /// @brief Get const reference to a vertex
  /// @param[in] i Vertex index
  /// @return Const reference to a vertex
  const auto& vertex(vertex_index i) const noexcept { return vertices_[i]; }

  /// @brief Get const reference to an edge
  /// @param[in] i Edge index
  /// @return Const reference to an edge
  const auto& edge(edge_index i) const noexcept { return edges_[i]; }

  /// @brief Get const reference to a face
  /// @param[in] i Face index
  /// @return Const reference to a face
  const auto& face(face_index i) const noexcept { return faces_[i]; }
  /// @}

  /// @brief Checks if this mesh has invalid mesh entities (vertex, edge, or
  /// face)
  bool has_invalid_entities() const noexcept {
    return has_invalid_vertices_ || has_invalid_edges_ || has_invalid_faces_;
  }

  /// @brief Removes invalid mesh entities from mesh
  void remove_invalid_entities();

 private:
  void update_face_connectivity(const Face& f, const face_index fi) noexcept;
  void update_edge_connectivity(const fmesh::edge& e,
                                const edge_index ei) noexcept;

  // Mesh entities
  vertex_property<Point> vertices_;
  edge_property<fmesh::edge> edges_;
  face_property<Face> faces_;

  // Mesh connectivity
  vertex_property<std::vector<vertex_index>> vertex_vertices_;
  vertex_property<std::vector<edge_index>> vertex_edges_;
  vertex_property<std::vector<face_index>> vertex_faces_;
  edge_property<std::vector<face_index>> edge_faces_;
  face_property<std::vector<edge_index>> face_edges_;

  // Is a mesh entity valid?
  // Mesh entities can be invalidated.
  // Call remove_invalid_entities() if you really need to remove them.
  vertex_property<bool> is_valid_vertex_;
  edge_property<bool> is_valid_edge_;
  face_property<bool> is_valid_face_;

  // Flags to check if there are any invalid mesh entities.
  bool has_invalid_vertices_;
  bool has_invalid_edges_;
  bool has_invalid_faces_;
};

template <typename Point, typename Face>
void fracture_mesh<Point, Face>::update_face_connectivity(
    const Face& f, const face_index fi) noexcept {
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

template <typename Point, typename Face>
void fracture_mesh<Point, Face>::update_edge_connectivity(
    const fmesh::edge& e, const edge_index ei) noexcept {
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

template <typename Point, typename Face>
void fracture_mesh<Point, Face>::remove_invalid_entities() {
  if (!this->has_invalid_entities()) return;

  if (has_invalid_vertices_) {
    //
  }

  if (has_invalid_edges_) {
    //
  }

  if (has_invalid_faces_) {
    //
  }
}

}  // namespace fmesh

#endif  // FMESH_FRACTURE_MESH_HPP