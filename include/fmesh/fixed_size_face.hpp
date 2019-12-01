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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef FMESH_FIXED_SIZE_FACE_HPP
#define FMESH_FIXED_SIZE_FACE_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <type_traits>

#include "fmesh/edge.hpp"
#include "fmesh/index.hpp"

namespace fmesh {

template <std::size_t N>
class fixed_size_face {
 public:
  static_assert(N >= 3, "N must be larger than 3.");

  fixed_size_face() = default;

  template <typename... Args>
  fixed_size_face(Args... args) : vertices_{args...} {
    static_assert(std::conjunction_v<std::is_same<VertexIndex, Args>...>,
                  "Argument must be VertexIndex.");
    static_assert(sizeof...(Args) == N,
                  "The number of arguments must be equal to N.");
  }

  constexpr auto size() const noexcept { return N; }

  auto& operator[](std::size_t i) noexcept {
    assert(i < N);
    return vertices_[i];
  }
  const auto& operator[](std::size_t i) const noexcept {
    assert(i < N);
    return vertices_[i];
  }

  auto begin() noexcept { return vertices_.begin(); }
  auto end() noexcept { return vertices_.end(); }
  auto begin() const noexcept { return vertices_.begin(); }
  auto end() const noexcept { return vertices_.end(); }

  auto* data() noexcept { return vertices_.data(); }
  const auto* data() const noexcept { return vertices_.data(); }

  std::array<edge, N> to_edges() const noexcept {
    std::array<edge, N> edges;
    for (std::size_t i = 0; i < N - 1; ++i)
      edges[i] = edge{vertices_[i], vertices_[i + 1]};
    edges[N - 1] = edge{vertices_[N - 1], vertices_[0]};
    return edges;
  }

  bool contains(vertex_index v) const noexcept {
    return std::find(this->begin(), this->end(), v) != this->end();
  }

  bool contains(const edge& e) const noexcept {
    for (std::size_t i = 0; i < N - 1; ++i)
      if (e == edge{vertices_[i], vertices_[i + 1]}) return true;
    if (e == edge{vertices_[N - 1], vertices_[0]}) return true;
    return false;
  }

  bool shares_vertex_with(const fixed_size_face& other) const {
    for (auto&& vi : *this) {
      for (auto&& vj : other) {
        if (vi == vj) return true;
      }
    }
    return false;
  }

  bool shares_edge_with(const fixed_size_face& other) const {
    const auto edges1 = this->to_edges();
    const auto edges2 = other.to_edges();
    for (auto&& e1 : edges1) {
      for (auto&& e2 : edges2) {
        if (e1 == e2) return true;
      }
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const fixed_size_face& face) {
    for (auto&& v : face) os << v << ' ';
    return os;
  }

  friend std::istream& operator>>(std::istream& is, fixed_size_face& face) {
    for (auto&& v : face) is >> v;
    return is;
  }

 private:
  std::array<vertex_index, N> vertices_;
};

using tri_face = fixed_size_face<3>;
using quad_face = fixed_size_face<4>;

}  // namespace fmesh

#endif  // FMESH_FIXED_SIZE_FACE_HPP