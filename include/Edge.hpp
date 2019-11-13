#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>

#include "Index.hpp"

namespace shirose {

struct Edge {
  VertexIndex first;
  VertexIndex second;

  Edge() = default;

  Edge(VertexIndex v1, VertexIndex v2) : first{v1}, second{v2} {}

  bool Contains(VertexIndex v) const noexcept {
    return v == this->first || v == this->second;
  }
  bool SharesVertexWith(const Edge& other) const noexcept {
    return other.Contains(first) || other.Contains(second);
  }
  bool IsValid() const noexcept { return first.IsValid() && second.IsValid(); }

  friend bool operator==(const Edge& e1, const Edge& e2) noexcept {
    return (e1.first == e2.first && e1.second == e2.second) ||
           (e1.first == e2.second && e1.second == e2.first);
  }
  friend bool operator!=(const Edge& e1, const Edge& e2) noexcept {
    return !(e1 == e2);
  }

  friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
    return os << e.first << ' ' << e.second;
  }
  friend std::istream& operator>>(std::istream& is, Edge& e) {
    return is >> e.first >> e.second;
  }
};

}  // namespace shirose

#endif  // EDGE_HPP