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
    for (auto&& ei : es)
      if (edges_[ei] == e) return ei;
    return edge_index{};
  }

  /// @brief Find the face index of a given face
  /// @param[in] f Face
  /// @return The index of a given face. If the face is not found, an invalid
  /// index is returned.
  face_index find(const Face& f) const noexcept {
    const auto& fs = vertex_faces_[f[0]];
    for (auto&& fi : fs)
      if (faces_[fi] == f) return fi;
    return face_index{};
  }

  /// @brief Add a new vertex to mesh
  /// @param[in] p A new vertex as a vector
  /// @return The index of a new vertex
  ///
  /// This function updates vertex connectivity info.
  vertex_index add(const Point& p);

  /// @brief Add a new face to mesh
  /// @param[in] f A new face
  /// @return The index of a new face
  ///
  /// This function updates vertex, edge, and face connectivity info.
  /// If the given face is already registered, an invalid face index will be
  /// returned.
  face_index add(const Face& f);

  /// @brief Invalidate a given vertex
  /// @param[in] vi Vertex index to be invalidated
  ///
  /// This function invalidates a given vertex. In addition, it also invalidates
  /// faces which contain the vertex. Isolated edges created by this operations
  /// are also invalidated.
  void invalidate(vertex_index vi);

  /// @brief Invalidate a given face
  /// @param[in] fi Face index to be invalidated
  ///
  /// This function invalidates a given face. In addition, it also invalidates
  /// isolated vertices and edges which consist of the face.
  void invalidate(face_index fi);

  /// @name Iterators
  /// @{

  /// @brief Returns the beginning of vertices
  auto vertex_begin() const noexcept {
    return vertex_iterator{vertex_index{0}};
  }

  /// @brief Returns the end of vertices
  auto vertex_end() const noexcept {
    return vertex_iterator{vertex_index{vertices_.size()}};
  }

  /// @brief Returns the beginning of edges
  auto edge_begin() const noexcept { return edge_iterator{edge_index{0}}; }

  /// @brief Returns the end of edges
  auto edge_end() const noexcept {
    return edge_iterator{edge_index{edges_.size()}};
  }

  /// @brief Returns the beginning of faces
  auto face_begin() const noexcept { return face_iterator{face_index{0}}; }

  /// @brief Returns the end of faces
  auto face_end() const noexcept {
    return face_iterator{face_index{faces_.size()}};
  }
  /// @}

  /// @name Ranges
  /// @{

  /// @brief Returns vertex range
  auto vertices() const noexcept {
    return make_iterator_range(this->vertex_begin(), this->vertex_end());
  }

  /// @brief Returns edge range
  auto edges() const noexcept {
    return make_iterator_range(this->edge_begin(), this->edge_end());
  }

  /// @brief Returns face range
  auto faces() const noexcept {
    return make_iterator_range(this->face_begin(), this->face_end());
  }
  /// @}

  /// @name Accessors
  /// @{

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
  /// @brief Update face connectivity data
  /// @param[in] f A new face
  /// @param[in] fi The index of a new face
  void update_face_connectivity(const Face& f, const face_index fi) noexcept;

  /// @brief Checks if a given edge is isolated
  /// @param[in] ei Edge index
  ///
  /// Isolated edges do not have any valid faces connected to themselves.
  bool is_isolated(const edge_index ei) const noexcept {
    const auto& fs = edge_faces_[ei];
    const auto num_valid_faces =
        std::count_if(fs.begin(), fs.end(),
                      [this](const auto fi) { return this->is_valid(fi); });
    return num_valid_faces == 0;
  }

  /// @brief Checks if a given vertex is isolated
  /// @param[in] vi Vertex index
  ///
  /// Isolated vertices do not have any valid faces connected to themselves.
  bool is_isolated(const vertex_index vi) const noexcept {
    const auto& fs = vertex_faces_[vi];
    const auto num_valid_faces =
        std::count_if(fs.begin(), fs.end(),
                      [this](const auto fi) { return this->is_valid(fi); });
    return num_valid_faces == 0;
  }

  /// @name Mesh entities
  /// @{
  vertex_property<Point> vertices_;
  edge_property<fmesh::edge> edges_;
  face_property<Face> faces_;
  /// @}

  /// @name Mesh connectivity
  /// @{
  vertex_property<std::vector<vertex_index>> vertex_vertices_;
  vertex_property<std::vector<edge_index>> vertex_edges_;
  vertex_property<std::vector<face_index>> vertex_faces_;
  edge_property<std::vector<face_index>> edge_faces_;
  face_property<std::vector<edge_index>> face_edges_;
  /// @}

  /// @name Validity of mesh entities
  ///
  /// Is a mesh entity valid?
  /// Mesh entities can be invalidated.
  /// Call remove_invalid_entities() if you really need to remove them.
  /// @{
  vertex_property<bool> is_valid_vertex_;
  edge_property<bool> is_valid_edge_;
  face_property<bool> is_valid_face_;
  /// @}

  /// @name Flags to check if there are any invalid mesh entities.
  /// @{
  bool has_invalid_vertices_;
  bool has_invalid_edges_;
  bool has_invalid_faces_;
  /// @}
};

template <typename Point, typename Face>
vertex_index fracture_mesh<Point, Face>::add(const Point& p) {
  const vertex_index vi{vertices_.size()};
  vertices_.push_back(p);
  const auto n = vertices_.size();
  vertex_vertices_.resize(n);
  vertex_edges_.resize(n);
  vertex_faces_.resize(n);
  is_valid_vertex_.push_back(true);
  return vi;
}

template <typename Point, typename Face>
face_index fracture_mesh<Point, Face>::add(const Face& f) {
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

template <typename Point, typename Face>
void fracture_mesh<Point, Face>::invalidate(vertex_index vi) {
  has_invalid_vertices_ = true;
  is_valid_vertex_[vi] = false;

  // Invalidate all edges connected to the vertex
  const auto& es = vertex_edges_[vi];
  for (auto&& ei : es) is_valid_edge_[ei] = false;

  // Invalidate all faces connected to the vertex
  const auto& fs = vertex_faces_[vi];
  for (auto&& fi : fs) is_valid_face_[fi] = false;

  // Invalidate isolated edges
  for (auto&& fi : fs) {
    const auto& es = face_edges_[fi];
    for (auto&& ei : es) {
      if (this->is_valid(ei) && this->is_isolated(ei))
        is_valid_edge_[ei] = false;
    }
  }
}

template <typename Point, typename Face>
void fracture_mesh<Point, Face>::invalidate(face_index fi) {
  has_invalid_faces_ = true;
  is_valid_face_[fi] = false;

  // Invalidate isolated edges
  const auto& es = face_edges_[fi];
  for (auto&& ei : es) {
    if (this->is_valid(ei) && this->is_isolated(ei)) is_valid_edge_[ei] = false;
  }

  // Invalidate isolated vertices
  const auto& f = faces_[fi];
  for (auto&& vi : f) {
    if (this->is_valid(vi) && this->is_isolated(vi))
      is_valid_vertex_[vi] = false;
  }
}

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
      // An edge is new
      // So first regiter the new edge, and then update all connectivity data.
      const edge_index ej{edges_.size()};
      edges_.push_back(e);
      edge_faces_.resize(edges_.size());
      is_valid_edge_.push_back(true);

      // Update vertex-edges and vertex-vertices
      vertex_edges_[e.first].push_back(ej);
      vertex_edges_[e.second].push_back(ej);
      vertex_vertices_[e.first].push_back(e.second);
      vertex_vertices_[e.second].push_back(e.first);

      // Update edge-faces and face-edges
      edge_faces_[ej].push_back(fi);
      face_edges_[fi].push_back(ej);
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