cmake_minimum_required(VERSION 3.17)
project(rogue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

include(CTest)

add_subdirectory(src/typelist)
add_subdirectory(src/algebra)
add_subdirectory(src/ecs)
add_subdirectory(src/game)
add_subdirectory(src/sdl)