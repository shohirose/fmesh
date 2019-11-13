#ifndef FIXED_SIZE_FACE_HPP
#define FIXED_SIZE_FACE_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <type_traits>

#include "Edge.hpp"
#include "Index.hpp"

namespace shirose {

template <std::size_t N>
class FixedSizeFace {
 public:
  static_assert(N >= 3, "N must be larger than 3.");

  FixedSizeFace() = default;

  template <typename... Args>
  FixedSizeFace(Args... args) : vertices_{args...} {
    static_assert(std::conjunction_v<std::is_same<VertexIndex, Args>...>,
                  "Argument must be VertexIndex.");
    static_assert(sizeof...(Args) == N,
                  "The number of arguments must be equal to N.");
  }

  constexpr auto Size() const noexcept { return N; }

  auto& operator[](std::size_t i) noexcept {
    assert(i < N);
    return vertices_[i];
  }
  const auto& operator[](std::size_t i) const noexcept {
    assert(i < N);
    return vertices_[i];
  }

  auto Begin() noexcept { return vertices_.begin(); }
  auto End() noexcept { return vertices_.end(); }
  auto Begin() const noexcept { return vertices_.begin(); }
  auto End() const noexcept { return vertices_.end(); }

  auto* Data() noexcept { return vertices_.data(); }
  const auto* Data() const noexcept { return vertices_.data(); }

  std::array<Edge, N> ToEdges() const noexcept {
    std::array<Edge, N> edges;
    for (std::size_t i = 0; i < N - 1; ++i)
      edges[i] = Edge{vertices_[i], vertices_[i + 1]};
    edges[N - 1] = Edge{vertices_[N - 1], vertices_[0]};
    return edges;
  }

  bool Contains(VertexIndex v) const noexcept {
    return std::find(this->begin(), this->end(), v) != this->end();
  }

  bool Contains(const Edge& e) const noexcept {
    for (std::size_t i = 0; i < N - 1; ++i)
      if (e == Edge{vertices_[i], vertices_[i + 1]}) return true;
    if (e == Edge{vertices_[N - 1], vertices_[0]}) return true;
    return false;
  }

  bool SharesVertexWith(const FixedSizeFace& other) const {
    for (auto&& vi : *this) {
      for (auto&& vj : other) {
        if (vi == vj) return true;
      }
    }
    return false;
  }

  bool SharesEdgeWith(const FixedSizeFace& other) const {
    const auto edges1 = this->ToEdges();
    const auto edges2 = other.ToEdges();
    for (auto&& e1 : edges1) {
      for (auto&& e2 : edges2) {
        if (e1 == e2) return true;
      }
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const FixedSizeFace& face) {
    for (auto&& v : face) os << v << ' ';
    return os;
  }

  friend std::istream& operator>>(std::istream& is, FixedSizeFace& face) {
    for (auto&& v : face) is >> v;
    return is;
  }

 private:
  std::array<VertexIndex, N> vertices_;
};

using TriFace = FixedSizeFace<3>;
using QuadFace = FixedSizeFace<4>;

}  // namespace shirose

#endif  // FIXED_SIZE_FACE_HPP