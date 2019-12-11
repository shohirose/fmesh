find_package(GTest REQUIRED)

function(add_unit_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
        GTest::GTest
        GTest::Main
        fmesh::fmesh
    )
    add_test(
        NAME ${name}
        COMMAND ${name}
    )
endfunction()

add_unit_test(test_fracture_mesh)
