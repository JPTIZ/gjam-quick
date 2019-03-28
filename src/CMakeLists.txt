cmake_minimum_required(VERSION 3.10)
project(sfml-test CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External Libraries
find_package(SFML 2.5 COMPONENTS graphics system)

if(PROFILE)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "-pg")
        set(CMAKE_C_FLAGS "-pg")
        set(CMAKE_EXE_FLAGS "-pg")
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXXFLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Werror")
endif()

add_subdirectory(game)
add_subdirectory(engine)
add_subdirectory(app)

foreach(_target ${TARGETS})
    message("Tarjeta: ${_target}")
    # set_property(TARGET ${_target} PROPERTY CXX_STANDARD 17)
endforeach()
