cmake_minimum_required(VERSION 3.13)

include_directories(
    ${PROJECT_SOURCE_DIR}/lib/
)

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
    ${PROJECT_SOURCE_DIR}/lib/file_reader.h
    ${PROJECT_SOURCE_DIR}/source/file_reader.c
)

add_executable(
    day5
    ${PROJECT_SOURCE_DIR}/source/day5.c
)