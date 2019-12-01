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

#ifndef FMESH_ITERATOR_RANGE_HPP
#define FMESH_ITERATOR_RANGE_HPP

#include <cassert>

#include "fmesh/index_iterator.hpp"

namespace fmesh {

template <typename Iterator>
class iterator_range {
 public:
  iterator_range() = default;
  iterator_range(const Iterator& begin, const Iterator& end)
      : begin_{begin}, end_{end} {}

  auto begin() const noexcept { return begin_; }
  auto end() const noexcept { return end_; }
  auto size() const noexcept {
    assert(begin_ <= end_);
    return static_cast<std::size_t>(std::distance(begin_, end_));
  }
  bool empty() const noexcept { return begin_ == end_; }

  friend bool operator==(const iterator_range& left,
                         const iterator_range& right) noexcept {
    return left.begin_ == right.begin_ && left.end_ == right.end_;
  }
  friend bool operator!=(const iterator_range& left,
                         const iterator_range& right) noexcept {
    return !(left == right);
  }

 private:
  Iterator begin_;
  Iterator end_;
};

template <typename Iterator>
iterator_range<Iterator> make_iterator_range(const Iterator& begin,
                                             const Iterator& end) {
  return {begin, end};
}

}  // namespace fmesh

#endif  // FMESH_ITERATOR_RANGE_HPP