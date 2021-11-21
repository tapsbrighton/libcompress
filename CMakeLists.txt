cmake_minimum_required(VERSION 3.17)
project(libcompress VERSION 1.3)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(libcompress STATIC libcompress.cpp include/libcompress.h include/libcompress_global.h)

target_compile_definitions(libcompress PRIVATE COMPRESS_LIBRARY)


set(PROJECT_SOURCES
        libcompress.cpp
        include/libcompress.h
        include/libcompress_global.h
        res.qrc

        )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/libcompress-1.3.cmake"
        VERSION 1.3
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS libcompress
        EXPORT libcompress
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime
        )

include(CMakePackageConfigHelpers)
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/libcompressConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/libcompressConfig.cmake"
        INSTALL_DESTINATION lib/cmake/libcompress
)

install(EXPORT libcompress DESTINATION lib/cmake/libcompress)
install(FILES "${PROJECT_BINARY_DIR}/libcompress-1.3.cmake"
        "${PROJECT_BINARY_DIR}/libcompressConfig.cmake"
        DESTINATION lib/cmake/libcompress)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

find_package(Qt5 COMPONENTS
        Core
        Widgets
        REQUIRED)

target_link_libraries(libcompress
        Qt5::Core
        Qt5::Widgets
        )
