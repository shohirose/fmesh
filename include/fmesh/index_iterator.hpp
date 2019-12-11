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

#ifndef FMESH_INDEX_ITERATOR_HPP
#define FMESH_INDEX_ITERATOR_HPP

#include <iterator>
#include <type_traits>

#include "fmesh/index.hpp"
#include "fmesh/type_traits.hpp"

namespace fmesh {

template <typename T,
          std::enable_if_t<is_index<T>::value, std::nullptr_t> = nullptr>
class index_iterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;

  index_iterator() = default;
  explicit index_iterator(T idx) : idx_{idx} {}

  index_iterator& operator++() noexcept {
    ++idx_;
    return *this;
  }
  index_iterator& operator--() noexcept {
    --idx_;
    return *this;
  }
  index_iterator operator++(int) noexcept {
    index_iterator tmp{*this};
    ++idx_;
    return tmp;
  }
  index_iterator operator--(int) noexcept {
    index_iterator tmp{*this};
    --idx_;
    return tmp;
  }

  index_iterator operator+=(difference_type n) noexcept {
    idx_ += n;
    return *this;
  }
  index_iterator operator-=(difference_type n) noexcept {
    idx_ -= n;
    return *this;
  }
  index_iterator operator+(difference_type n) const noexcept {
    return index_iterator{idx_ + n};
  }
  index_iterator operator-(difference_type n) const noexcept {
    return index_iterator{idx_ - n};
  }

  friend bool operator==(const index_iterator& it1,
                         const index_iterator& it2) noexcept {
    return it1.idx_ == it2.idx_;
  }
  friend bool operator!=(const index_iterator& it1,
                         const index_iterator& it2) noexcept {
    return !(it1 == it2);
  }
  friend bool operator<(const index_iterator& it1,
                        const index_iterator& it2) noexcept {
    return it1.idx_ < it2.idx_;
  }
  friend bool operator>(const index_iterator& it1,
                        const index_iterator& it2) noexcept {
    return it1.idx_ > it2.idx_;
  }
  friend bool operator<=(const index_iterator& it1,
                         const index_iterator& it2) noexcept {
    return it1.idx_ <= it2.idx_;
  }
  friend bool operator>=(const index_iterator& it1,
                         const index_iterator& it2) noexcept {
    return it1.idx_ >= it2.idx_;
  }

  const T& operator*() const noexcept { return idx_; }
  T& operator*() noexcept { return idx_; }

  const T* operator->() const noexcept { return &idx_; }
  T* operator->() noexcept { return &idx_; }

 private:
  T idx_;
};

using vertex_iterator = index_iterator<vertex_index>;
using edge_iterator = index_iterator<edge_index>;
using face_iterator = index_iterator<face_index>;

}  // namespace fmesh

#endif  // FMESH_INDEX_ITERATOR_HPP