cmake_minimum_required(VERSION 3.31)

project(MKBrowser LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(WARNING "Consider setting CMAKE_TOOLCHAIN_FILE to your vcpkg toolchain file for dependency management!")
endif()

# Find raylib package
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)


include_directories(${raylib_INCLUDE_DIRS})

add_executable(MKBrowser
    src/main.cc
    src/UrlParser.cc
    src/Url.cc
    src/Util.cc
)

target_link_libraries(MKBrowser PRIVATE raylib glfw)

# Platform-specific linking for raylib
if(APPLE)
    target_link_libraries(MKBrowser PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
    )
endif()
