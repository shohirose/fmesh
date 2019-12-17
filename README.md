# fmesh

`fmesh` is a header-only mesh library to handle fracture surfaces written in C++.

## Motivation

When I started to write a hydraulic fracturing simulator as a part of my research, there is no open source library for surface meshes which allows fractures to branch. [OpenMesh](https://www.openmesh.org/) and [Surface Mesh](https://doc.cgal.org/latest/Surface_mesh/index.html) are only surface mesh libraries I found but either of them does not support branching surfaces because of their half-edge mesh structure. Therefore, I had to write my own mesh class specialized for fracture surfaces for my dissertation. This library is an extension of the mesh class.

## Features

- Header-only.
- Supports both triangular and quadrilateral faces. Only uniform meshes are currently supported.
- Supports fracture branching.
- Type-safe indices (`vertex_index`/`edge_index`/`face_index`) to access mesh entities (vertices, edges, and faces).
- Range-based for loops for mesh entities.
- `property_array` for mapping mesh indices to corresponding mesh properties like [Boost.PropertyMap](https://www.boost.org/doc/libs/1_49_0/libs/property_map/doc/property_map.html).
- `property_registry` for easy access to mesh properties.

## Requirements

- C++17 (might reduce this requirement in the future.)

## Basic Examples

```cpp
#include <vector>
#include "fmesh/fixed_size_face.hpp"
#include "fmesh/fracture_mesh.hpp"

using namespace fmesh;

struct point {
  double x;
  double y;
  double z;

  point() = default;
  point(double tx, double ty, double tz) : x{tx}, y{ty}, z{tz} {}
};

int main() {}
  fracture_mesh<point, tri_face> mesh;

  // Add vertices
  std::vector<vertex_index> v_ids;
  v_ids.push_back(mesh.add_vertex(0.0, 0.0, 0.0));
  v_ids.push_back(mesh.add_vertex(1.0, 0.0, 0.0));
  v_ids.push_back(mesh.add_vertex(0.0, 0.0, 1.0));
  v_ids.push_back(mesh.add_vertex(1.0, 0.0, 1.0));
  v_ids.push_back(mesh.add_vertex(0.0, 0.0, 2.0));
  v_ids.push_back(mesh.add_vertex(2.0, 0.0, 0.5));

  // Add faces
  std::vector<face_index> f_ids;
  f_ids.push_back(mesh.add_face(v_ids[0], v_ids[1], v_ids[2]));
  f_ids.push_back(mesh.add_face(v_ids[1], v_ids[3], v_ids[2]));
  f_ids.push_back(mesh.add_face(v_ids[2], v_ids[3], v_ids[4]));
  f_ids.push_back(mesh.add_face(v_ids[1], v_ids[5], v_ids[3]));

  // Access to a point
  const auto& p1 = mesh.vertex(v_idx[0]);

  // Access to a face
  const auto& f1 = mesh.face(f_idx[0]);

  // Range-based for
  for (auto&& vi : mesh.vertices()) {
      const auto& p = mesh.vertex(vi);
      // ...
  }

  for (auto&& ei : mesh.edges()) {
      const auto& e = mesh.edge(ei);
      // ...
  }

  for (auto&& fi : mesh.faces()) {
      const auto& f = mesh.face(fi);
      // ...
  }

  // Invalidate vertices and faces
  mesh.invalidate(v_ids[0]);
  mesh.invalidate(f_ids[1]);

  // Remove invalid mesh entities
  mesh.remove_invalid_entities();
}
```

## Planned Features

- Supports heterogeneous meshes (faces can have different number of vertices).
- I/O to VTK formats.
- Implements `fracture_mesh<...>::remove_invalid_entities()` function.
- Mesh intersection algorithms.

