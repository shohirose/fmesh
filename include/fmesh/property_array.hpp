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

#ifndef FMESH_PROPERTY_ARRAY_HPP
#define FMESH_PROPERTY_ARRAY_HPP

#include <cassert>
#include <vector>

#include "fmesh/index.hpp"
#include "fmesh/type_traits.hpp"
#include "fmesh/allocator.hpp"

namespace fmesh {

template <typename Key>
struct property_array_base {
  virtual ~property_array_base() = default;
};

template <typename Key, typename T, typename Allocator = allocator<T>>
class property_array : public property_array_base<Key> {
 public:
  static_assert(is_index<Key>::value, "Key must be Index type.");

  using base_type = property_array_base<Key>;

  property_array() = default;
  property_array(std::size_t size) : base_type{}, values_{size} {}
  property_array(std::initializer_list<T> list) : base_type{}, values_{list} {}
  property_array(const property_array&) = default;
  property_array(property_array&&) = default;

  virtual ~property_array() override = default;

  property_array& operator=(const property_array&) = default;
  property_array& operator=(property_array&&) = default;

  bool empty() const noexcept { return values_.empty(); }
  auto size() const noexcept { return values_.size(); }
  void resize(std::size_t size) { values_.resize(size); }
  void reserve(std::size_t capacity) { values_.reserve(capacity); }
  void clear() { values_.clear(); }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    values_.emplace_back(std::forward<Args>(args)...);
  }

  void push_back(const T& t) { values_.push_back(t); }

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
using vertex_property = property_array<vertex_index, T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using edge_property = property_array<edge_index, T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using face_property = property_array<face_index, T, Allocator>;

}  // namespace fmesh

#endif  // FMESH_PROPERTY_ARRAY_HPP