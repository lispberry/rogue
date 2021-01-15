#include <console.hpp>

using namespace rogue;
int main() {
    console::Window window("rogue", 640, 480);
    console::render_matrix(
        window,
        console::colored("Hello, world!\nTest line", console::color::black, console::color::white));
    SDL_Delay(5000);

    return 0;
}