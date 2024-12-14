cmake_minimum_required(VERSION 3.13)
project(advent_of_code_2024_C)

add_subdirectory(${PROJECT_SOURCE_DIR}/source)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

include_directories(${CMAKE_SOURCE_DIR}/include)
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

add_executable(
    day8
    ${PROJECT_SOURCE_DIR}/source/day8.c
    
    ${PROJECT_SOURCE_DIR}/source/my_string.c
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
)

add_executable(
    day9
    ${PROJECT_SOURCE_DIR}/source/day9.c
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
    ${PROJECT_SOURCE_DIR}/source/vector.c
)

add_executable(
    day10
    ${PROJECT_SOURCE_DIR}/source/day10.c
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
    ${PROJECT_SOURCE_DIR}/source/my_string.c
)

add_executable(
    day11
    ${PROJECT_SOURCE_DIR}/source/day11.c
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
    ${PROJECT_SOURCE_DIR}/source/my_hash_table.c
    )
target_link_libraries(day11 m)

add_executable(
    day12
    ${PROJECT_SOURCE_DIR}/source/day12.c
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
    ${PROJECT_SOURCE_DIR}/source/my_string.c
)

include(CTest)

add_executable(
    vector_test 
    test/vector_test.c
    source/vector.c)
    add_test(vector_test_build
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target vector_test
  )
  set_tests_properties(vector_test_build PROPERTIES FIXTURES_SETUP    test_fixture)
  add_test(vector_test vector_test)
  set_tests_properties(vector_test       PROPERTIES FIXTURES_REQUIRED test_fixture)

  add_executable(
    hash_table_test 
    test/hash_table_test.c
    source/my_hash_table.c)
    add_test(hash_table_test_build
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target hash_table_test
  )
  set_tests_properties(hash_table_test_build PROPERTIES FIXTURES_SETUP    test_fixture)
  add_test(hash_table_test hash_table_test)
  set_tests_properties(hash_table_test       PROPERTIES FIXTURES_REQUIRED test_fixture)

  add_executable(
    linalg_test 
    test/linalg_test.c
    source/my_linalg.c)
    add_test(linalg_build
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target linalg_test
  )
  set_tests_properties(linalg_build PROPERTIES FIXTURES_SETUP    test_fixture)
  add_test(linalg_test linalg_test)
  set_tests_properties(linalg_test       PROPERTIES FIXTURES_REQUIRED test_fixture)