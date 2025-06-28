cmake_minimum_required(VERSION 3.29)
project(RunTime)

set(CMAKE_CXX_STANDARD 20)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Core/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include)

if(WIN32)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/win64)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/win64)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core)
endif()


add_executable(RunTime src/main.cpp)

target_link_libraries(RunTime SDL3 Core glad)
