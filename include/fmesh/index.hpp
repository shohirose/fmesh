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

#ifndef FMESH_INDEX_HPP
#define FMESH_INDEX_HPP

#include <cstddef>
#include <iostream>
#include <limits>

namespace fmesh {

template <typename Tag>
class index {
 public:
  static constexpr auto default_value = std::numeric_limits<std::size_t>::max();

  index() = default;
  explicit index(std::size_t idx) : idx_{idx} {}

  explicit operator std::size_t() const noexcept { return idx_; }

  auto get() const noexcept { return idx_; }
  void set(std::size_t idx) noexcept { idx_ = idx; }
  void reset() noexcept { idx_ = default_value; }
  bool is_valid() const noexcept { return idx_ != default_value; }

  index& operator--() noexcept {
    --idx_;
    return *this;
  }
  index operator--(int) noexcept {
    index tmp(*this);
    --idx_;
    return tmp;
  }

  index& operator++() noexcept {
    ++idx_;
    return *this;
  }
  index operator++(int) noexcept {
    index tmp(*this);
    ++idx_;
    return tmp;
  }

  index& operator-=(std::size_t n) noexcept {
    idx_ -= n;
    return *this;
  }
  index& operator+=(std::size_t n) noexcept {
    idx_ += n;
    return *this;
  }

  friend bool operator==(index i, index j) noexcept { return i.idx_ == j.idx_; }
  friend bool operator!=(index i, index j) noexcept { return i.idx_ != j.idx_; }
  friend bool operator<(index i, index j) noexcept { return i.idx_ < j.idx_; }
  friend bool operator>(index i, index j) noexcept { return i.idx_ > j.idx_; }
  friend bool operator<=(index i, index j) noexcept { return i.idx_ <= j.idx_; }
  friend bool operator>=(index i, index j) noexcept { return i.idx_ >= j.idx_; }

  friend std::ostream& operator<<(std::ostream& os, const index& i) {
    return os << i.idx_;
  }
  friend std::istream& operator>>(std::istream& is, index& i) {
    return is >> i.idx_;
  }

 private:
  std::size_t idx_ = default_value;
};

struct vertex_tag {};
struct edge_tag {};
struct face_tag {};

using vertex_index = index<vertex_tag>;
using edge_index = index<edge_tag>;
using face_index = index<face_tag>;

}  // namespace fmesh

#endif  // FMESH_INDEX_HPP