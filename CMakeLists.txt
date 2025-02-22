cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MKBrowser)

add_executable(MKBrowser src/main.cc src/url-parser.cc src/url.cc src/util.cc)