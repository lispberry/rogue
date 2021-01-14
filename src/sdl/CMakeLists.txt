find_package(SDL2)

include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_TTF REQUIRED SDL2_ttf)

add_library(sdl STATIC src/console.cpp include/console.hpp)
target_include_directories(sdl PRIVATE ${SDL2_INCLUDE_DIRS} {SDL2_TTF_INCLUDE_DIRS})
target_include_directories(sdl PUBLIC include/)
target_link_libraries(sdl ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} algebra)