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

#ifndef FMESH_VECTOR_HPP
#define FMESH_VECTOR_HPP

#include <Eigen/Core>

namespace fmesh {

template <typename Scalar, int N>
class vector {};

template <typename Scalar>
class vector<Scalar, 2> : public Eigen::Matrix<Scalar, 2, 1> {
 public:
  using base_type = Eigen::Matrix<Scalar, 2, 1>;

  static constexpr bool REQUIRES_ALIGNED_ALLOC = (sizeof(base_type) % 16 == 0);

  vector() = default;
  vector(Scalar x, Scalar y) : base_type{x, y} {}

  template <typename Derived>
  vector(const Eigen::EigenBase<Derived>& other) : base_type{other} {}

  template <typename Derived>
  vector& operator=(const Eigen::EigenBase<Derived>& other) {
    this->base_type::operator=(other);
    return *this;
  }

  auto squared_norm() const noexcept { return this->base_type::squaredNorm(); }
  vector normalized() const noexcept { return {this->base_type::normalized()}; }
};

template <typename Scalar>
class vector<Scalar, 3> : public Eigen::Matrix<Scalar, 3, 1> {
 public:
  using base_type = Eigen::Matrix<Scalar, 3, 1>;

  static constexpr bool REQUIRES_ALIGNED_ALLOC = (sizeof(base_type) % 16 == 0);

  vector() = default;
  vector(Scalar x, Scalar y, Scalar z) : base_type{x, y, z} {}

  template <typename Derived>
  vector(const Eigen::EigenBase<Derived>& other) : base_type{other} {}

  template <typename Derived>
  vector& operator=(const Eigen::EigenBase<Derived>& other) {
    this->base_type::operator=(other);
    return *this;
  }

  auto squared_norm() const noexcept { return this->base_type::squaredNorm(); }
  vector normalized() const noexcept { return {this->base_type::normalized()}; }
};

using vector2d = vector<double, 2>;
using vector3d = vector<double, 3>;

}  // namespace fmesh

#endif  // FMESH_VECTOR_HPP