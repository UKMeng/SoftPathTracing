cmake_minimum_required(VERSION 3.28)
project(SoftPathTracing)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)

add_executable(SoftPathTracing ${source})

target_include_directories(SoftPathTracing PUBLIC src/Math)
