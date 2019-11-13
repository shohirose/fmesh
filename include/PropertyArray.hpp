#ifndef PROPERTY_ARRAY_HPP
#define PROPERTY_ARRAY_HPP

#include <cassert>
#include <vector>

#include "Index.hpp"
#include "TypeTraits.hpp"

namespace shirose {

template <typename Key>
struct PropertyArrayBase {
  virtual ~PropertyArrayBase() = default;
};

template <typename Key, typename T, typename Allocator = std::allocator<T>>
class PropertyArray : public PropertyArrayBase<Key> {
 public:
  static_assert(IsIndex<Key>::value, "Key must be Index type.");

  using BaseType = PropertyArrayBase<Key>;

  PropertyArray() = default;
  PropertyArray(std::size_t size) : BaseType{}, values_{size} {}
  PropertyArray(std::initializer_list<T> list) : BaseType{}, values_{list} {}
  PropertyArray(const PropertyArray&) = default;
  PropertyArray(PropertyArray&&) = default;

  virtual ~PropertyArray() override = default;

  PropertyArray& operator=(const PropertyArray&) = default;
  PropertyArray& operator=(PropertyArray&&) = default;

  bool Empty() const noexcept { return values_.empty(); }
  auto Size() const noexcept { return values_.size(); }
  void Resize(std::size_t size) { values_.resize(size); }
  void Reserve(std::size_t capacity) { values_.reserve(capacity); }
  void Clear() { values_.clear(); }

  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    values_.emplace_back(std::forward<Args>(args)...);
  }

  void PushBack(const T& t) { values_.push_back(t); }

  auto& operator[](Key key) noexcept {
    assert(static_cast<std::size_t>(key) < values_.size());
    return values_[static_cast<std::size_t>(key)];
  }
  const auto& operator[](Key key) const noexcept {
    assert(static_cast<std::size_t>(key) < values_.size());
    return values_[static_cast<std::size_t>(key)];
  }

 private:
  std::vector<T, Allocator> values_;
};

template <typename T, typename Allocator = std::allocator<T>>
using VertexProperty = PropertyArray<VertexIndex, T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using EdgeProperty = PropertyArray<EdgeIndex, T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using FaceProperty = PropertyArray<FaceIndex, T, Allocator>;

}  // namespace shirose

#endif  // PROPERTY_ARRAY_HPP