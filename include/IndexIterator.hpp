#ifndef INDEX_ITERATOR_HPP
#define INDEX_ITERATOR_HPP

#include <iterator>
#include <type_traits>

#include "Index.hpp"
#include "TypeTraits.hpp"

namespace shirose {

template <typename T,
          std::enable_if_t<IsIndex<T>::value, std::nullptr_t> = nullptr>
class IndexIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;

  IndexIterator() = default;
  IndexIterator(T idx) : idx_{idx} {}

  IndexIterator& operator++() noexcept {
    ++idx_;
    return *this;
  }
  IndexIterator& operator--() noexcept {
    --idx_;
    return *this;
  }
  IndexIterator operator++(int) noexcept {
    IndexIterator tmp{*this};
    ++idx_;
    return tmp;
  }
  IndexIterator operator--(int) noexcept {
    IndexIterator tmp{*this};
    --idx_;
    return tmp;
  }

  IndexIterator operator+=(difference_type n) noexcept {
    idx_ += n;
    return *this;
  }
  IndexIterator operator-=(difference_type n) noexcept {
    idx_ -= n;
    return *this;
  }
  IndexIterator operator+(difference_type n) const noexcept {
    return IndexIterator{idx_ + n};
  }
  IndexIterator operator-(difference_type n) const noexcept {
    return IndexIterator{idx_ - n};
  }

  friend bool operator==(const IndexIterator& it1,
                         const IndexIterator& it2) noexcept {
    return it1.idx_ == it2.idx_;
  }
  friend bool operator!=(const IndexIterator& it1,
                         const IndexIterator& it2) noexcept {
    return !(it1 == it2);
  }

  const T& operator*() const noexcept { return idx_; }
  T& operator*() noexcept { return idx_; }

  const T* operator->() const noexcept { return &idx_; }
  T* operator->() noexcept { return &idx_; }

 private:
  T idx_;
};

using VertexIterator = IndexIterator<VertexIndex>;
using EdgeIterator = IndexIterator<EdgeIndex>;
using FaceIterator = IndexIterator<FaceIndex>;

}  // namespace shirose

#endif  // INDEX_ITERATOR_HPP