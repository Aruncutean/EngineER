cmake_minimum_required(VERSION 3.29)
project(Core)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/bullet3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3-image/include)

file(GLOB GLM_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/glm/*.hpp
)


if(IOS)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/macos/arm64-ios)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/macos/arm64-ios)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/macos/arm64-ios)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/bullet3/lib/macos/arm64-ios)

    find_library(UIKIT_FRAMEWORK UIKit)
    find_library(FOUNDATION_FRAMEWORK Foundation)
    find_library(AVFOUNDATION_FRAMEWORK AVFoundation)
    find_library(COREVIDEO_FRAMEWORK CoreVideo)
    find_library(COREMEDIA_FRAMEWORK CoreMedia)
    find_library(AUDIO_TOOLBOX_FRAMEWORK AudioToolbox)
    find_library(GAMECONTROLLER_FRAMEWORK GameController)
    find_library(METAL_FRAMEWORK Metal)

    set(IOS_FRAMEWORKS
            ${UIKIT_FRAMEWORK}
            ${FOUNDATION_FRAMEWORK}
            ${AVFOUNDATION_FRAMEWORK}
            ${COREVIDEO_FRAMEWORK}
            ${COREMEDIA_FRAMEWORK}
            ${AUDIO_TOOLBOX_FRAMEWORK}
            ${GAMECONTROLLER_FRAMEWORK}
            ${METAL_FRAMEWORK}
    )

elseif(ANDROID)
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
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3-image/lib/win64)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/debug)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/bullet3/lib/win64/debug)
    else()
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/release)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/bullet3/lib/win64/release)
    endif()

elseif(APPLE)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/macos/x64-osx)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/macos/x64-osx)


    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(AVFOUNDATION_FRAMEWORK AVFoundation)
    find_library(COREMEDIA_FRAMEWORK CoreMedia)
    find_library(COREVIDEO_FRAMEWORK CoreVideo)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(GAMECONTROLLER_FRAMEWORK GameController)
    find_library(AUDIO_TOOLBOX_FRAMEWORK AudioToolbox)
    find_library(CORE_AUDIO_FRAMEWORK CoreAudio)
    find_library(METAL_FRAMEWORK Metal)
    find_library(FOUNDATION_FRAMEWORK Foundation)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore)

    set(MACOS_FRAMEWORKS
            ${COCOA_FRAMEWORK}
            ${AVFOUNDATION_FRAMEWORK}
            ${COREMEDIA_FRAMEWORK}
            ${COREVIDEO_FRAMEWORK}
            ${IOKIT_FRAMEWORK}
            ${GAMECONTROLLER_FRAMEWORK}
            ${AUDIO_TOOLBOX_FRAMEWORK}
            ${CORE_AUDIO_FRAMEWORK}
            ${METAL_FRAMEWORK}
            ${FOUNDATION_FRAMEWORK}
            ${QUARTZCORE_FRAMEWORK}
    )
endif()

file(GLOB FILE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/service/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/esc/process/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/shader/*.cpp
)

file(GLOB_RECURSE FILE_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

add_library(Core STATIC ${FILE_SRC} ${FILE_HEADERS} ${GLM_SRC})

target_link_libraries(Core
        SDL3
        glad
        SDL3_image

        $<$<CONFIG:Debug>:boost_system-vc143-mt-gd-x64-1_86>
        $<$<CONFIG:Release>:boost_system-vc143-mt-x64-1_86>

        $<$<CONFIG:Debug>:boost_filesystem-vc143-mt-gd-x64-1_86>
        $<$<CONFIG:Release>:boost_filesystem-vc143-mt-x64-1_86>

        $<$<CONFIG:Debug>:Bullet3Collision_Debug>
        $<$<CONFIG:Release>:BulletCollision>

        $<$<CONFIG:Debug>:Bullet3Dynamics_Debug>
        $<$<CONFIG:Release>:BulletDynamics>

        $<$<CONFIG:Debug>:LinearMath_Debug>
        $<$<CONFIG:Release>:LinearMath>

)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    target_link_libraries(Core ${IOS_FRAMEWORKS})
elseif(APPLE)
    target_link_libraries(Core ${MACOS_FRAMEWORKS})
endif()