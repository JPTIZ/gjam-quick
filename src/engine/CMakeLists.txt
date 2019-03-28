cmake_minimum_required(VERSION 3.0)
project(engine CXX)

add_library(engine
    STATIC
        engine.cpp
)

target_compile_features(engine
    PRIVATE
        cxx_std_17
)

target_compile_options(engine
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
)

target_compile_definitions(engine
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        stdc++fs
        sfml-graphics
)
