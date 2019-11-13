#ifndef PROPERTY_REGISTRY_HPP
#define PROPERTY_REGISTRY_HPP

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "PropertyArray.hpp"

namespace shirose {

template <typename Key>
class PropertyRegistry {
 public:
  bool CheckIn(PropertyArrayBase<Key>* arrayPtr, const std::string& name) {
    if (this->Contains(name)) {
      std::cerr << "Property [" << name << "] already regsitered." << std::endl;
      return false;
    }
    return registry_.emplace(name, arrayPtr).second;
  }

  bool CheckOut(const std::string& name) { return registry_.erase(name) > 0; }

  template <typename T, typename Allocator = std::allocator<T>>
  PropertyArray<Key, T, Allocator>& Lookup(const std::string& name) {
    auto it = registry_.find(name);
    if (it == registry_.end()) {
      std::stringstream ss;
      ss << "Property [" << name << "] not found.";
      throw std::invalid_argument{ss.str()};
    }

    using ArrayType = PropertyArray<Key, T, Allocator>;
    if (auto p = dynamic_cast<ArrayType*>(it->second)) return *p;

    std::stringstream ss;
    ss << "Dynamic cast to " << typeid(ArrayType).name() << " failed.";
    throw std::invalid_argument(ss.str());
  }

  template <typename T, typename Allocator = std::allocator<T>>
  const PropertyArray<Key, T, Allocator>& Lookup(
      const std::string& name) const {
    auto it = registry_.find(name);
    if (it == registry_.end()) {
      std::stringstream ss;
      ss << "Property [" << name << "] not found.";
      throw std::invalid_argument{ss.str()};
    }

    using ArrayType = PropertyArray<Key, T, Allocator>;
    if (auto p = dynamic_cast<ArrayType*>(it->second)) return *p;

    std::stringstream ss;
    ss << "Dynamic cast to " << typeid(ArrayType).name() << " failed.";
    throw std::invalid_argument(ss.str());
  }

  bool Contains(const std::string& name) const {
    return registry_.find(name) != registry_.end();
  }

 private:
  std::unordered_map<std::string, PropertyArrayBase<Key>*> registry_;
};

using FacePropertyRegistry = PropertyRegistry<FaceIndex>;
using EdgePropertyRegistry = PropertyRegistry<EdgeIndex>;
using VertexPropertyRegistry = PropertyRegistry<VertexIndex>;

}  // namespace shirose

#endif  // PROPERTY_DREGISTRY_HPP