cmake_minimum_required(VERSION 3.29)
project(Core)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
if(ANDROID)
    add_library(SDL3 SHARED IMPORTED)
    set_target_properties(SDL3 PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/android/${ANDROID_ABI}/libSDL3.so"
    )

    add_library(gald SHARED IMPORTED)
    set_target_properties(gald PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/android/${ANDROID_ABI}/libglad.a"
    )
elseif(WIN32)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/win64)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/win64)
endif()

add_library(Core STATIC src/Window.cpp include/Window.hpp
        include/GameWindow.hpp src/GameWindow.cpp)

target_link_libraries(Core SDL3 glad)