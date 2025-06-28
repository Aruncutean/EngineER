if(NOT TARGET SDL3::Headers)
add_library(SDL3::Headers INTERFACE IMPORTED)
set_target_properties(SDL3::Headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/arunc/.gradle/caches/8.13/transforms/37fceea904773f369bf9944e947ca5fd/transformed/SDL3-3.2.16/prefab/modules/Headers/include"
    INTERFACE_LINK_LIBRARIES "SDL3::SDL3-Headers"
)
endif()

if(NOT TARGET SDL3::SDL3)
add_library(SDL3::SDL3 INTERFACE IMPORTED)
set_target_properties(SDL3::SDL3 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/arunc/.gradle/caches/8.13/transforms/37fceea904773f369bf9944e947ca5fd/transformed/SDL3-3.2.16/prefab/modules/SDL3/include"
    INTERFACE_LINK_LIBRARIES "SDL3::SDL3-shared"
)
endif()

if(NOT TARGET SDL3::SDL3-Headers)
add_library(SDL3::SDL3-Headers INTERFACE IMPORTED)
set_target_properties(SDL3::SDL3-Headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/arunc/.gradle/caches/8.13/transforms/37fceea904773f369bf9944e947ca5fd/transformed/SDL3-3.2.16/prefab/modules/SDL3-Headers/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

if(NOT TARGET SDL3::SDL3-shared)
add_library(SDL3::SDL3-shared SHARED IMPORTED)
set_target_properties(SDL3::SDL3-shared PROPERTIES
    IMPORTED_LOCATION "C:/Users/arunc/.gradle/caches/8.13/transforms/37fceea904773f369bf9944e947ca5fd/transformed/SDL3-3.2.16/prefab/modules/SDL3-shared/libs/android.x86_64/libSDL3.so"
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/arunc/.gradle/caches/8.13/transforms/37fceea904773f369bf9944e947ca5fd/transformed/SDL3-3.2.16/prefab/modules/SDL3-shared/include"
    INTERFACE_LINK_LIBRARIES "SDL3::Headers"
)
endif()

if(NOT TARGET SDL3::SDL3_test)
add_library(SDL3::SDL3_test STATIC IMPORTED)
set_target_properties(SDL3::SDL3_test PROPERTIES
    IMPORTED_LOCATION "C:/Users/arunc/.gradle/caches/8.13/transforms/37fceea904773f369bf9944e947ca5fd/transformed/SDL3-3.2.16/prefab/modules/SDL3_test/libs/android.x86_64/libSDL3_test.a"
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/arunc/.gradle/caches/8.13/transforms/37fceea904773f369bf9944e947ca5fd/transformed/SDL3-3.2.16/prefab/modules/SDL3_test/include"
    INTERFACE_LINK_LIBRARIES "SDL3::Headers"
)
endif()

