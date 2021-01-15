add_library(algebra INTERFACE)
target_include_directories(algebra INTERFACE include/)
target_link_libraries(algebra INTERFACE typelist)