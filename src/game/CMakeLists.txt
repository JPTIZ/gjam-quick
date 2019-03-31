cmake_minimum_required(VERSION 3.0)
project(game CXX)

add_library(game
    STATIC
        intro.cpp
        game.cpp
)

target_compile_features(game
    PRIVATE
        cxx_std_20
)

target_compile_options(game
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
)

target_compile_definitions(game
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(game
    PUBLIC
        ${engine_SOURCE_DIR}/..
        ${engine_SOURCE_DIR}
        ${game_SOURCE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        engine
    PRIVATE
        sfml-graphics
)
