#ifndef ITERATOR_RANGE_HPP
#define ITERATOR_RANGE_HPP

#include <cassert>

#include "IndexIterator.hpp"

namespace shirose {

template <typename Iterator>
class IteratorRange {
 public:
  IteratorRange() = default;
  IteratorRange(const Iterator& begin, const Iterator& end)
      : begin_{begin}, end_{end} {}

  auto Begin() const noexcept { return begin_; }
  auto End() const noexcept { return end_; }
  auto Size() const noexcept {
    assert(begin_ <= end_);
    return static_cast<std::size_t>(std::distance(begin_, end_));
  }
  bool Empty() const noexcept { return begin_ == end_; }

  friend bool operator==(const IteratorRange& left,
                         const IteratorRange& right) noexcept {
    return left.begin_ == right.begin_ && left.end_ == right.end_;
  }
  friend bool operator!=(const IteratorRange& left,
                         const IteratorRange& right) noexcept {
    return !(left == right);
  }

 private:
  Iterator begin_;
  Iterator end_;
};

template <typename Iterator>
IteratorRange<Iterator> MakeIteratorRange(const Iterator& begin,
                                          const Iterator& end) {
  return {begin, end};
}

}  // namespace shirose

#endif  // ITERATOR_RANGE_HPP