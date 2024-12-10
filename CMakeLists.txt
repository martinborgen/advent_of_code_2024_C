cmake_minimum_required(VERSION 3.13)
project(advent_of_code_2024_C)

add_subdirectory(${PROJECT_SOURCE_DIR}/source)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/test)


include(CTest)

add_executable(
    vector_test 
    test/vector_test.c
    source/vector.c)
add_test(test_build
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target vector_test
)
set_tests_properties(test_build PROPERTIES FIXTURES_SETUP    test_fixture)
add_test(vector_test vector_test)
set_tests_properties(vector_test       PROPERTIES FIXTURES_REQUIRED test_fixture)