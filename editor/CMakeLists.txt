cmake_minimum_required(VERSION 3.29)
project(Editor)

set(CMAKE_CXX_STANDARD 20)


add_executable(Editor src/main.cpp)

target_link_libraries(Editor )