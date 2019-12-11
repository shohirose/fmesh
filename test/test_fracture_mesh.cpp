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

#include <gtest/gtest.h>
#include <vector>
#include "fmesh/fixed_size_face.hpp"
#include "fmesh/fracture_mesh.hpp"
#include "fmesh/vector.hpp"

using namespace fmesh;

TEST(FmeshTest, FractureMeshTest) {
  fracture_mesh<vector3d, tri_face> mesh;

  std::vector<vertex_index> v_ids;
  v_ids.push_back(mesh.add_vertex(0.0, 0.0, 0.0));
  v_ids.push_back(mesh.add_vertex(1.0, 0.0, 0.0));
  v_ids.push_back(mesh.add_vertex(0.0, 0.0, 1.0));
  v_ids.push_back(mesh.add_vertex(1.0, 0.0, 1.0));
  v_ids.push_back(mesh.add_vertex(0.0, 0.0, 2.0));
  v_ids.push_back(mesh.add_vertex(2.0, 0.0, 0.5));

  std::vector<face_index> f_ids;
  f_ids.push_back(mesh.add_face(v_ids[0], v_ids[1], v_ids[2]));
  f_ids.push_back(mesh.add_face(v_ids[1], v_ids[3], v_ids[2]));
  f_ids.push_back(mesh.add_face(v_ids[2], v_ids[3], v_ids[4]));
  f_ids.push_back(mesh.add_face(v_ids[1], v_ids[5], v_ids[3]));

  EXPECT_FALSE(mesh.has_invalid_entities());

  mesh.invalidate(f_ids[0]);

  EXPECT_TRUE(mesh.has_invalid_entities());
  EXPECT_FALSE(mesh.is_valid(f_ids[0]));
  EXPECT_FALSE(mesh.is_valid(v_ids[0]));
  EXPECT_TRUE(mesh.is_valid(v_ids[1]));
  EXPECT_TRUE(mesh.is_valid(v_ids[2]));

  mesh.invalidate(f_ids[1]);

  EXPECT_FALSE(mesh.is_valid(f_ids[1]));
  EXPECT_TRUE(mesh.is_valid(v_ids[1]));
  EXPECT_TRUE(mesh.is_valid(v_ids[2]));
  EXPECT_TRUE(mesh.is_valid(v_ids[3]));
}