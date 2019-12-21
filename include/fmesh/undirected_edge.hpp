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

#ifndef FMESH_UNDIRECTED_EDGE_HPP
#define FMESH_UNDIRECTED_EDGE_HPP

#include <iostream>

#include "fmesh/index.hpp"

namespace fmesh {

struct undirected_edge {
  vertex_index first;
  vertex_index second;

  undirected_edge() = default;

  undirected_edge(vertex_index v1, vertex_index v2) : first{v1}, second{v2} {}

  bool contains(vertex_index v) const noexcept {
    return v == this->first || v == this->second;
  }
  bool shares_vertex_with(const undirected_edge& other) const noexcept {
    return other.contains(first) || other.contains(second);
  }
  bool is_valid() const noexcept {
    return first.is_valid() && second.is_valid();
  }

  friend bool operator==(const undirected_edge& e1,
                         const undirected_edge& e2) noexcept {
    return (e1.first == e2.first && e1.second == e2.second) ||
           (e1.first == e2.second && e1.second == e2.first);
  }
  friend bool operator!=(const undirected_edge& e1,
                         const undirected_edge& e2) noexcept {
    return !(e1 == e2);
  }

  friend std::ostream& operator<<(std::ostream& os, const undirected_edge& e) {
    return os << e.first << ' ' << e.second;
  }
  friend std::istream& operator>>(std::istream& is, undirected_edge& e) {
    return is >> e.first >> e.second;
  }
};

}  // namespace fmesh

#endif  // FMESH_UNDIRECTED_EDGE_HPP