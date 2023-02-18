cmake_minimum_required(VERSION 3.0.0)
project(compiler VERSION 0.0.1 LANGUAGES C)

add_executable(${PROJECT_NAME} main.c)

add_subdirectory(scripts)
include_directories(${CMAKE_SOURCE_DIR}/scripts)
target_link_libraries(${PROJECT_NAME} helpers)