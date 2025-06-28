if(NOT TARGET jsoncpp::jsoncpp)
add_library(jsoncpp::jsoncpp STATIC IMPORTED)
set_target_properties(jsoncpp::jsoncpp PROPERTIES
    IMPORTED_LOCATION "C:/Users/arunc/.gradle/caches/8.13/transforms/a0a243a0ac75b055471ae0c212ec7766/transformed/jsoncpp-1.9.6/prefab/modules/jsoncpp/libs/android.arm64-v8a/libjsoncpp.a"
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/arunc/.gradle/caches/8.13/transforms/a0a243a0ac75b055471ae0c212ec7766/transformed/jsoncpp-1.9.6/prefab/modules/jsoncpp/libs/android.arm64-v8a/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

