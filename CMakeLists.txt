cmake_minimum_required(VERSION 3.13)

project(fmesh CXX)

cmake_policy(SET CMP0076 NEW)

find_package(Eigen3 REQUIRED)

add_library(fmesh)
target_compile_definitions(fmesh
  PUBLIC
  )
target_compile_features(fmesh
  PUBLIC cxx_std_17
  )
target_compile_options(fmesh
  PUBLIC -Wall -Wextra -Wpedantic
  )
target_include_directories(fmesh
  PUBLIC include
  )
target_link_libraries(fmesh
  PUBLIC Eigen3::Eigen
  )

add_library(fmesh::fmesh ALIAS fmesh)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(test)