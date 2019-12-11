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

#ifndef FMESH_PROPERTY_ARRAY_HPP
#define FMESH_PROPERTY_ARRAY_HPP

#include <cassert>
#include <vector>

#include "fmesh/index.hpp"
#include "fmesh/type_traits.hpp"

namespace fmesh {

template <typename Key>
struct property_array_base {
  virtual ~property_array_base() = default;
};

template <typename Key, typename T, typename Allocator = std::allocator<T>>
class property_array : public property_array_base<Key> {
 public:
  static_assert(is_index<Key>::value, "Key must be an index type.");

  using array_type = std::vector<T, Allocator>;
  using value_type = typename array_type::value_type;
  using allocator_type = typename array_type::allocator_type;
  using size_type = typename array_type::size_type;
  using difference_type = typename array_type::difference_type;
  using reference = typename array_type::reference;
  using const_reference = typename array_type::const_reference;
  using pointer = typename array_type::pointer;
  using const_pointer = typename array_type::const_pointer;
  using iterator = typename array_type::iterator;
  using const_iterator = typename array_type::const_iterator;
  using reverse_iterator = typename array_type::reverse_iterator;
  using const_reverse_iterator = typename array_type::const_reverse_iterator;

  using base_type = property_array_base<Key>;

  property_array() = default;
  property_array(size_type size) : base_type{}, values_{size} {}
  property_array(std::initializer_list<T> list) : base_type{}, values_{list} {}
  property_array(const property_array&) = default;
  property_array(property_array&&) = default;

  virtual ~property_array() override = default;

  property_array& operator=(const property_array&) = default;
  property_array& operator=(property_array&&) = default;

  bool empty() const noexcept { return values_.empty(); }
  size_type size() const noexcept { return values_.size(); }
  void resize(size_type size) { values_.resize(size); }
  void reserve(size_type capacity) { values_.reserve(capacity); }
  size_type capacity() const noexcept { return values_.capacity(); }
  void clear() { values_.clear(); }

  iterator begin() noexcept { return values_.begin(); }
  const_iterator begin() const noexcept { return values_.begin(); }
  iterator end() noexcept { return values_.end(); }
  const_iterator end() const noexcept { return values_.end(); }

  reverse_iterator rbegin() noexcept { return values_.rbegin(); }
  const_reverse_iterator rbegin() const noexcept { return values_.rbegin(); }
  reverse_iterator rend() noexcept { return values_.rend(); }
  const_reverse_iterator rend() const noexcept { return values_.rend(); }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    values_.emplace_back(std::forward<Args>(args)...);
  }

  void push_back(const T& t) { values_.push_back(t); }
  void push_back(T&& t) { values_.push_back(std::move(t)); }

  reference operator[](Key key) noexcept {
    assert(static_cast<std::size_t>(key) < values_.size());
    return values_[static_cast<std::size_t>(key)];
  }
  const_reference operator[](Key key) const noexcept {
    assert(static_cast<std::size_t>(key) < values_.size());
    return values_[static_cast<std::size_t>(key)];
  }

 private:
  array_type values_;
};

template <typename T, typename Allocator = std::allocator<T>>
using vertex_property = property_array<vertex_index, T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using edge_property = property_array<edge_index, T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using face_property = property_array<face_index, T, Allocator>;

}  // namespace fmesh

#endif  // FMESH_PROPERTY_ARRAY_HPP