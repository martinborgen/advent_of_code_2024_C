cmake_minimum_required(VERSION 3.13)
project(advent_of_code_2024_C)

add_subdirectory(${PROJECT_SOURCE_DIR}/source)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/test)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options("-Wall")

add_executable(
    day1
    ${PROJECT_SOURCE_DIR}/source/day1.c
)

add_executable(
    day2
    ${PROJECT_SOURCE_DIR}/source/day2.c
)

add_executable(
    day3
    ${PROJECT_SOURCE_DIR}/source/day3.c
)

add_executable(
    day4
    ${PROJECT_SOURCE_DIR}/source/day4.c
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
)

add_executable(
    day5
    ${PROJECT_SOURCE_DIR}/source/day5.c
    ${PROJECT_SOURCE_DIR}/source/vector.c
)

add_executable(
    day6
    ${PROJECT_SOURCE_DIR}/source/day6.c
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
    ${PROJECT_SOURCE_DIR}/source/my_string.c
)

add_executable(
    day7
    ${PROJECT_SOURCE_DIR}/source/day7.c
    ${PROJECT_SOURCE_DIR}/source/vector.c

)


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