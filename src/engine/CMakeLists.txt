cmake_minimum_required(VERSION 3.0)
project(engine CXX)

set(SOURCES
    engine.cpp
)

add_library(engine STATIC
    ${SOURCES}
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        ${SFML_INCLUDE_DIR}
)

link_directories(
    ${SFML_LIBRARY_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
)
