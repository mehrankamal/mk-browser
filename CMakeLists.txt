cmake_minimum_required(VERSION 3.31)

project(MKBrowser LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(WARNING "Consider setting CMAKE_TOOLCHAIN_FILE to your vcpkg toolchain file for dependency management!")
endif()

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${raylib_INCLUDE_DIRS})

add_subdirectory(Libraries/LibBrowser)
add_subdirectory(Applications/Browser)
