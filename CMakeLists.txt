cmake_minimum_required(VERSION 3.13)

project(fmesh CXX)

cmake_policy(SET CMP0076 NEW)

add_library(fmesh INTERFACE)
target_compile_features(fmesh
  INTERFACE cxx_std_17
  )
target_compile_options(fmesh
  INTERFACE -Wall -Wextra -Wpedantic
  )
target_include_directories(fmesh
  INTERFACE include
  )

add_library(fmesh::fmesh ALIAS fmesh)

add_subdirectory(include)
add_subdirectory(test)