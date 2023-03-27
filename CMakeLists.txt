cmake_minimum_required(VERSION 3.0.0)
project(compiler VERSION 0.0.1 LANGUAGES C)

add_executable(${PROJECT_NAME} main.c)

if (${CMAKE_BUILD_TYPE} STREQUAL "")
    message(WARNING "CMAKE_BUILD_TYPE is not defined, setting it as Release")
    set(CMAKE_BUILD_TYPE Release)
else()
    message(STATUS "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE} mode")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DEBUG ON)
    set(INFO ON)
endif()

if (DEBUG)
    add_definitions(-DDEBUG)
endif()

if (INFO)
    add_definitions(-DINFO)
endif()

add_subdirectory(scripts)
include_directories(${CMAKE_SOURCE_DIR}/scripts)
target_link_libraries(${PROJECT_NAME} Helpers Compiler)