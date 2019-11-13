#ifndef INDEX_HPP
#define INDEX_HPP

#include <cstddef>
#include <iostream>
#include <limits>

namespace shirose {

template <typename Tag>
class Index {
 public:
  static constexpr auto defaultValue = std::numeric_limits<std::size_t>::max();

  Index() = default;
  explicit Index(std::size_t idx) : idx_{idx} {}

  explicit operator std::size_t() const noexcept { return idx_; }

  auto Get() const noexcept { return idx_; }
  void Set(std::size_t idx) noexcept { idx_ = idx; }
  void Reset() noexcept { idx_ = defaultValue; }
  bool IsValid() const noexcept { return idx_ != defaultValue; }

  Index& operator--() noexcept {
    --idx_;
    return *this;
  }
  Index operator--(int) noexcept {
    Index tmp(*this);
    --idx_;
    return tmp;
  }

  Index& operator++() noexcept {
    ++idx_;
    return *this;
  }
  Index operator++(int) noexcept {
    Index tmp(*this);
    ++idx_;
    return tmp;
  }

  Index& operator-=(std::size_t n) noexcept {
    idx_ -= n;
    return *this;
  }
  Index& operator+=(std::size_t n) noexcept {
    idx_ += n;
    return *this;
  }

  friend bool operator==(Index i, Index j) noexcept { return i.idx_ == j.idx_; }
  friend bool operator!=(Index i, Index j) noexcept { return i.idx_ != j.idx_; }
  friend bool operator<(Index i, Index j) noexcept { return i.idx_ < j.idx_; }
  friend bool operator>(Index i, Index j) noexcept { return i.idx_ > j.idx_; }
  friend bool operator<=(Index i, Index j) noexcept { return i.idx_ <= j.idx_; }
  friend bool operator>=(Index i, Index j) noexcept { return i.idx_ >= j.idx_; }

  friend std::ostream& operator<<(std::ostream& os, const Index& i) {
    return os << i.idx_;
  }
  friend std::istream& operator>>(std::istream& is, Index& i) {
    return is >> i.idx_;
  }

 private:
  std::size_t idx_ = defaultValue;
};

struct VertexTag {};
struct EdgeTag {};
struct FaceTag {};

using VertexIndex = Index<VertexTag>;
using EdgeIndex = Index<EdgeTag>;
using FaceIndex = Index<FaceTag>;

}  // namespace shirose

#endif  // INDEX_HPP