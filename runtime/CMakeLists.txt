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
elseif(APPLE)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/macos/x64-osx)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/macos/x64-osx)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core)

    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(FOUNDATION_FRAMEWORK Foundation)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(COREAUDIO_FRAMEWORK CoreAudio)
    find_library(AUDIO_TOOLBOX_FRAMEWORK AudioToolbox)
    find_library(AVFOUNDATION_FRAMEWORK AVFoundation)
    find_library(COREMEDIA_FRAMEWORK CoreMedia)
    find_library(GAMECONTROLLER_FRAMEWORK GameController)
    find_library(UNIFORMTYPEIDENTIFIERS_FRAMEWORK UniformTypeIdentifiers)
    find_library(COREHAPTICS_FRAMEWORK CoreHaptics)
    find_library(APPLICATIONSERVICES_FRAMEWORK ApplicationServices)

    find_library(CARBON_FRAMEWORK Carbon)
    find_library(FORCEFEEDBACK_FRAMEWORK ForceFeedback)
    find_library(HID_FRAMEWORK HID)
endif()

add_executable(RunTime src/main.cpp)

target_link_libraries(RunTime SDL3 Core glad)

if(APPLE)

    target_link_libraries(RunTime
            ${COCOA_FRAMEWORK}
            ${IOKIT_FRAMEWORK}
            ${FOUNDATION_FRAMEWORK}
            ${COREFOUNDATION_FRAMEWORK}
            ${COREAUDIO_FRAMEWORK}
            ${AUDIO_TOOLBOX_FRAMEWORK}
            ${AVFOUNDATION_FRAMEWORK}
            ${COREMEDIA_FRAMEWORK}
            ${GAMECONTROLLER_FRAMEWORK}
            ${UNIFORMTYPEIDENTIFIERS_FRAMEWORK}
            ${COREHAPTICS_FRAMEWORK}
            ${APPLICATIONSERVICES_FRAMEWORK}
            ${FORCEFEEDBACK_FRAMEWORK}
            ${CARBON_FRAMEWORK}
    )
endif()