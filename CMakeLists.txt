cmake_minimum_required(VERSION 3.19)
project(libcompress)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

include_directories(/home/mine/headers)

add_library(libcompress SHARED libcompress.cpp include/libcompress.h include/libcompress_global.h)

target_compile_definitions(libcompress PRIVATE COMPRESS_LIBRARY)

find_package(Qt5 COMPONENTS
        Core
        Widgets
        REQUIRED)

add_library(libutility SHARED IMPORTED )
set_target_properties(libutility PROPERTIES IMPORTED_LOCATION "/home/mine/libraries/libutility.so")


target_link_libraries(libcompress
        Qt5::Core
        Qt5::Widgets
        libutility
        )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/libcompress-1.3.cmake"
        VERSION 1.3
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS libcompress
        EXPORT libcompress
        LIBRARY DESTINATION "/home/mine/libraries" COMPONENT Runtime
        ARCHIVE DESTINATION "/home/mine/libraries" COMPONENT Development
        RUNTIME DESTINATION "/home/mine/libraries" COMPONENT Runtime
        PUBLIC_HEADER DESTINATION "/home/mine/headers" COMPONENT Development
        BUNDLE DESTINATION "/home/mine/headers" COMPONENT Runtime
        )

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/libcompressConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/libcompressConfig.cmake"
    INSTALL_DESTINATION /home/mine/libraries/cmake/libcompress
)


install(EXPORT libcompress DESTINATION /home/mine/libraries/cmake/libcompress)

install(FILES "${PROJECT_BINARY_DIR}/libcompress-1.3.cmake"
        "${PROJECT_BINARY_DIR}/libcompressConfig.cmake"
        DESTINATION /home/mine/libraries/cmake/libcompress)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION /home/mine/headers)

set_target_properties(libcompress
                      PROPERTIES ARCHIVE_OUT_DIRECTORY "/home/mine/libraries"
                      BINARY_DIR "/home/mine/libraries"
                      #                      LIBRARY_OUTPUT_DIRECTORY "/home/mine/libraries"
                      ARCHIVE_OUTPUT_NAME "compress"
                      #                      RUNTIME_OUTPUT_DIRECTORY "/home/mine/libraries"
                      LIBRARY_OUTPUT_NAME "compress"
                      OUTPUT_NAME utility
                      )