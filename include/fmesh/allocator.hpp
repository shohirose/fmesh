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

#ifndef FMESH_ALLOCATOR_HPP
#define FMESH_ALLOCATOR_HPP

#include <Eigen/StdVector>
#include <memory>
#include <type_traits>
#include "fmesh/vector.hpp"

namespace fmesh {

namespace detail {

template <typename T>
struct allocator_selector {
  using type = std::allocator<T>;
};

template <typename T, int N>
struct allocator_selector<vector<T, N>> {
  using type = typename std::conditional<vector<T, N>::REQUIRES_ALIGNED_ALLOC,
                                         Eigen::aligned_allocator<vector<T, N>>,
                                         std::allocator<T>>::type;
};

}  // namespace detail

template <typename T>
using allocator = typename detail::allocator_selector<T>::type;

}  // namespace fmesh

#endif  // FMESH_ALLOCATOR_HPP