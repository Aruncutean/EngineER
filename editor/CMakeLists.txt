cmake_minimum_required(VERSION 3.29)
project(Editor)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Core/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/tinyfiledialogs/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3-image/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/bullet3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/fbx/include)

file(GLOB GLM_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/glm/*.hpp
)

if(WIN32)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/win64)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/win64)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/lib/win64)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/tinyfiledialogs/lib/win64)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3-image/lib/win64)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/debug)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/fbx/lib/2020/win64/debug)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/bullet3/lib/win64/debug)
    else()

        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/release)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/fbx/lib/2020/win64/release)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/bullet3/lib/win64/release)
    endif()


#    file(GLOB IMGUI_SRC
#            ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/include/*.cpp
#    )
#
#    file(GLOB IMGUI_HEADERS
#            ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/include/*.h
#    )
#
#    list(APPEND IMGUI_SRC
#            ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/include/backends/imgui_impl_sdl3.cpp
#            ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/include/backends/imgui_impl_opengl3.cpp
#    )
#
#    add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_HEADERS}  )
#
#    target_include_directories(imgui PUBLIC
#            ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/include
#            ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/include/backends
#    )

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

file(GLOB_RECURSE  FILE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE FILE_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)


add_executable(Editor  ${GLM_SRC}  ${FILE_SRC} ${FILE_HEADERS})

target_link_libraries(Editor
        SDL3
        SDL3_image
        Core
        glad
        imgui
        imguizmo
        opengl32
        tinyfiledialogs

        $<$<CONFIG:Debug>:libfbxsdk.lib>
        $<$<CONFIG:Release>:libfbxsdk.lib>

        $<$<CONFIG:Debug>:boost_system-vc143-mt-gd-x64-1_86>
        $<$<CONFIG:Release>:boost_system-vc143-mt-x64-1_86>

        $<$<CONFIG:Debug>:boost_filesystem-vc143-mt-gd-x64-1_86>
        $<$<CONFIG:Release>:boost_filesystem-vc143-mt-x64-1_86>

        $<$<CONFIG:Debug>:BulletCollision_Debug>
        $<$<CONFIG:Release>:BulletCollision>

        $<$<CONFIG:Debug>:BulletDynamics_Debug>
        $<$<CONFIG:Release>:BulletDynamics>

        $<$<CONFIG:Debug>:LinearMath_Debug>
        $<$<CONFIG:Release>:LinearMath>

)


if(APPLE)
    target_link_libraries(Editor
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

add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3/lib/win64/SDL3.dll
        $<TARGET_FILE_DIR:Editor>
)

add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/tinyfiledialogs/lib/win64/tinyfiledialogs.lib
        $<TARGET_FILE_DIR:Editor>
)

add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/lib/win64/glad.lib
        $<TARGET_FILE_DIR:Editor>
)

add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/sdl3-image/lib/win64/SDL3_image.lib
        $<TARGET_FILE_DIR:Editor>
)

add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/debug/boost_system-vc143-mt-gd-x64-1_86.lib>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/release/boost_system-vc143-mt-x64-1_86.lib>
        $<TARGET_FILE_DIR:Editor>
)

add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/debug/boost_filesystem-vc143-mt-gd-x64-1_86.lib>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/lib/win64/release/boost_filesystem-vc143-mt-x64-1_86.lib>
        $<TARGET_FILE_DIR:Editor>
)

add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../external/fbx/lib/2020/win64/debug/libfbxsdk.dll>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../external/fbx/lib/2020/win64/release/libfbxsdk.dll>

        $<TARGET_FILE_DIR:Editor>
)