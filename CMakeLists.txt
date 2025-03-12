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

add_executable(mk-browser
    Applications/Browser/main.cc
    Libraries/LibBrowser/UrlParser.cc
    Libraries/LibBrowser/Url.cc
    Libraries/LibBrowser/Util.cc
    Libraries/LibBrowser/Browser.cc
)

target_link_libraries(mk-browser PRIVATE
    raylib
    glfw
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Platform-specific linking for raylib
if(APPLE)
    target_link_libraries(mk-browser PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
    )
endif()
