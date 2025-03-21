add_library(LibBrowser STATIC
    UrlParser.cc
    HtmlParser.cc
    Url.cc
    Browser.cc
)

target_include_directories(LibBrowser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(LibBrowser PRIVATE
    raylib
    glfw
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Platform-specific linking for raylib
if(APPLE)
    target_link_libraries(LibBrowser PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
    )
endif()
