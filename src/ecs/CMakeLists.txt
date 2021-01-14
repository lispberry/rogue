project(ecs)

find_package(Boost 1.61)

add_subdirectory(test)

add_library(ecs INTERFACE)
target_link_libraries(ecs INTERFACE Boost::boost)
target_include_directories(ecs INTERFACE include)