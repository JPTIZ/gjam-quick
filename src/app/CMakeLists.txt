cmake_minimum_required(VERSION 3.0)
project(app)

add_executable(app
    main.cpp
)

target_compile_features(app
    PRIVATE
        cxx_std_17
)

target_compile_options(app
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
)

target_compile_definitions(app
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_link_libraries(app
    PUBLIC
        engine
        game
        stdc++fs
        sfml-window
        sfml-system
)

target_include_directories(app
    PUBLIC
        ${engine_SOURCE_DIR}
        ${game_SOURCE_DIR}
)
