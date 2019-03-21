cmake_minimum_required(VERSION 3.0)
project(app CXX)

set(SOURCE
    main.cpp
)

add_executable(app ${SOURCE})

target_link_libraries(app
    PUBLIC
        engine
        game
        ${SFML_LIBRARIES}
)

target_include_directories(app
    PUBLIC
        ${engine_SOURCE_DIR}
        ${game_SOURCE_DIR}
        ${SFML_INCLUDE_DIR}
)
