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