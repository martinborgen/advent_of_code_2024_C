cmake_minimum_required(VERSION 3.13)
project(advent_of_code_2024_C)

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/test)

add_executable(
    day1
    ${PROJECT_SOURCE_DIR}/day1.c
)

add_executable(
    day2
    ${PROJECT_SOURCE_DIR}/day2.c
)

add_executable(
    day3
    ${PROJECT_SOURCE_DIR}/day3.c
)

add_executable(
    day4
    ${PROJECT_SOURCE_DIR}/day4.c
    ${PROJECT_SOURCE_DIR}/file_reader.c
)

add_executable(
    day5
    ${PROJECT_SOURCE_DIR}/day5.c
)