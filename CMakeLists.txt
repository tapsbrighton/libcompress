cmake_minimum_required(VERSION 3.17)
project(libcompress VERSION 1.3)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets)


add_library(compress SHARED libcompress.cpp include/libcompress.h include/libcompress_global.h)

target_compile_definitions(compress PRIVATE COMPRESS_LIBRARY)


set(PROJECT_SOURCES
        libcompress.cpp
        include/libcompress.h
        include/libcompress_global.h

        )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/libcompress-1.3.cmake"
        VERSION 1.3
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS compress
        EXPORT compress
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

install(EXPORT compress DESTINATION lib/cmake/libcompress)
install(FILES "${PROJECT_BINARY_DIR}/libcompress-1.3.cmake"
        "${PROJECT_BINARY_DIR}/libcompressConfig.cmake"
        DESTINATION lib/cmake/libcompress)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(compress ${REQUIRED_LIBS_QUALIFIED})
