add_executable(mk-browser
    main.cc
)

target_link_libraries(mk-browser PRIVATE
    LibBrowser
)
