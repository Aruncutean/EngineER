cmake_minimum_required(VERSION 3.6)
project(main LANGUAGES CXX)

find_package(SDL3 REQUIRED CONFIG)

add_library(zlib SHARED IMPORTED)

set_target_properties(zlib PROPERTIES
        IMPORTED_LOCATION  "${CMAKE_CURRENT_SOURCE_DIR}/../extern/zlib/lib/${ANDROID_ABI}/libz.so"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../extern/zlib/include"
)

add_library(cxx_shared SHARED IMPORTED)
set_target_properties(cxx_shared PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../extern/lib/${ANDROID_ABI}/libc++_shared.so"
)

set(BOOST_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../extern/boost/lib/${ANDROID_ABI}")


set(BOOST_MODULES
        atomic chrono container context contract coroutine
        date_time  fiber filesystem graph iostreams json log log_setup
        math_c99 math_c99f math_c99l math_tr1 math_tr1l nowide
        prg_exec_monitor program_options random regex serialization
        stacktrace_basic stacktrace_noop system thread timer
        type_erasure unit_test_framework url wave wserialization
)


foreach(MODULE IN LISTS BOOST_MODULES)
    set(LIB_NAME "boost_${MODULE}")
    set(SO_FILE "${BOOST_LIB_DIR}/lib${LIB_NAME}.so")

    add_library(${LIB_NAME} SHARED IMPORTED)
    set_target_properties(${LIB_NAME} PROPERTIES
            IMPORTED_LOCATION "${SO_FILE}"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../extern/boost/include"
    )
endforeach()

add_library(boost_exception STATIC IMPORTED)
set_target_properties(boost_exception PROPERTIES
        IMPORTED_LOCATION "${BOOST_LIB_DIR}/libboost_exception.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../extern/boost/include"
)

add_library(glad SHARED IMPORTED)
set_target_properties(glad PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../extern/glad/lib/${ANDROID_ABI}/libglad.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../extern/glad/include"
)

set(BULLET_MODULES
        BulletCollision
        BulletDynamics
        BulletSoftBody
        Bullet3Common
        LinearMath
)

foreach(MODULE IN LISTS BULLET_MODULES)
    set(LIB_NAME "bullet_${MODULE}")
    set(SO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../extern/bullet3/lib/${ANDROID_ABI}/lib${MODULE}.a")

    add_library(${LIB_NAME} SHARED IMPORTED)
    set_target_properties(${LIB_NAME} PROPERTIES
            IMPORTED_LOCATION "${SO_FILE}"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../extern/bullet3/include"
    )
endforeach()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../glm")

add_library(core SHARED IMPORTED)

set_target_properties(core PROPERTIES
        IMPORTED_LOCATION  "${CMAKE_CURRENT_SOURCE_DIR}/../myLib/lib/${ANDROID_ABI}/libCore.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../myLib/include"
)


add_library(main SHARED
        YourSourceHere.cpp
)
target_link_libraries(main PRIVATE SDL3::SDL3 zlib
        core
        cxx_shared
        glad
       )

foreach(MODULE IN LISTS BOOST_MODULES)
    target_link_libraries(main PRIVATE boost_${MODULE})
endforeach()
target_link_libraries(main PRIVATE boost_exception)
# Bullet
foreach(MODULE IN LISTS BULLET_MODULES)
    target_link_libraries(main PRIVATE bullet_${MODULE})
endforeach()
