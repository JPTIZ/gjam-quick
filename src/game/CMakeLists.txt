cmake_minimum_required(VERSION 3.0)
project(game CXX)

set(SOURCES
    game.cpp
)

add_library(game
    STATIC
        ${SOURCES}
)

target_include_directories(game
    PUBLIC
        ${engine_SOURCE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        ${SFML_INCLUDE_DIR}
)

link_directories(
    ${SFML_LIBRARY_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        sfml-graphics
)
