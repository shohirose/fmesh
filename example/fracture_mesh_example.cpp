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

#include "fmesh/fixed_size_face.hpp"
#include "fmesh/fracture_mesh.hpp"
#include "fmesh/vector.hpp"

using namespace fmesh;

int main() {
  fracture_mesh<vector3d, tri_face> mesh;

  const vector3d p1(0, 0, 0);
  const vector3d p2(1, 0, 0);
  const vector3d p3(0, 0, 1);

  const auto v1 = mesh.add_vertex(p1);
  const auto v2 = mesh.add_vertex(p2);
  const auto v3 = mesh.add_vertex(p3);

  const tri_face f1{v1, v2, v3};

  const auto f1_idx = mesh.add_face(f1);

  std::cout << "face [" << f1_idx << "]: " << f1 << std::endl;
}