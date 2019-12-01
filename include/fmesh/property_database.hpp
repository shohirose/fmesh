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

#ifndef FMESH_PROPERTY_REGISTRY_HPP
#define FMESH_PROPERTY_REGISTRY_HPP

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "fmesh/property_array.hpp"

namespace fmesh {

template <typename Key>
class property_registry {
 public:
  bool checkin(property_array_base<Key>* array_ptr, const std::string& name) {
    if (this->Contains(name)) {
      std::cerr << "Property [" << name << "] already regsitered." << std::endl;
      return false;
    }
    return registry_.emplace(name, array_ptr).second;
  }

  bool checkout(const std::string& name) { return registry_.erase(name) > 0; }

  template <typename T, typename Allocator = std::allocator<T>>
  property_array<Key, T, Allocator>& lookup(const std::string& name) {
    auto it = registry_.find(name);
    if (it == registry_.end()) {
      std::stringstream ss;
      ss << "Property [" << name << "] not found.";
      throw std::invalid_argument{ss.str()};
    }

    using array_type = property_array<Key, T, Allocator>;
    if (auto p = dynamic_cast<array_type*>(it->second)) return *p;

    std::stringstream ss;
    ss << "Dynamic cast to " << typeid(array_type).name() << " failed.";
    throw std::invalid_argument(ss.str());
  }

  template <typename T, typename Allocator = std::allocator<T>>
  const property_array<Key, T, Allocator>& lookup(
      const std::string& name) const {
    auto it = registry_.find(name);
    if (it == registry_.end()) {
      std::stringstream ss;
      ss << "Property [" << name << "] not found.";
      throw std::invalid_argument{ss.str()};
    }

    using array_type = property_array<Key, T, Allocator>;
    if (auto p = dynamic_cast<array_type*>(it->second)) return *p;

    std::stringstream ss;
    ss << "Dynamic cast to " << typeid(array_type).name() << " failed.";
    throw std::invalid_argument(ss.str());
  }

  bool contains(const std::string& name) const {
    return registry_.find(name) != registry_.end();
  }

 private:
  std::unordered_map<std::string, property_array_base<Key>*> registry_;
};

using face_property_registry = property_registry<face_index>;
using edge_property_registry = property_registry<edge_index>;
using vertex_property_registry = property_registry<vertex_index>;

}  // namespace fmesh

#endif  // FMESH_PROPERTY_DREGISTRY_HPP