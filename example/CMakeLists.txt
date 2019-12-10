add_executable(fracture_mesh_example
  fracture_mesh_example.cpp
  )

target_link_libraries(fracture_mesh_example
  PUBLIC
    fmesh::fmesh
  )