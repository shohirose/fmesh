#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

#include "Index.hpp"

namespace shirose {

template <typename T>
struct IsIndex : std::false_type {};

template <typename T>
struct IsIndex<Index<T>> : std::true_type {};

}  // namespace shirose

#endif  // TYPE_TRAITS_HPP