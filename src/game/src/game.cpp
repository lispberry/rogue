#include <console.hpp>
#include <algebra.hpp>
//#include <ecs.hpp>

using namespace rogue;

//struct positioned {
//    algebra::size_t2 position;
//};
//
//struct displayed {
//    console::colored_char character;
//};
//
//struct display {
//    console::colored_field field;
//};
//
//struct window_container {
//    console::window window;
//};
//
//void place_characters(ecs::entity<positioned, displayed> &subject, ecs::entity<display> &display) {
//    display.field(subject.position) = subject.character;
//}
//
//void render_field(ecs::entity<display, window_container> &display) {
//    console::render_field(display.window, display.field);
//    display.field.fill(console::colored_char{static_cast<uint16_t>(' ')});
//}

int main() {
    console::sdl sdl{};

    console::window window("rogue", 640, 480);
    console::render_field(
        window,
        console::colored("Hello, world!\nTest line", console::color::black, console::color::white));
    SDL_Delay(5000);

    // create systems from place_characters and render_field

//    manager.add(ecs::entity{
//        positioned{algebra::vector{0ul, 0ul}},
//        displayed{{'@', console::color::white, console::color::transparent}}
//    });
//
//    manager.add(ecs::entity{
//        positioned{algebra::vector{1ul, 2ul}},
//        displayed{{'t', console::color::white, console::make_color(0, 192, 0)}}
//    });
//
//    manager.add(ecs::entity{
//        display{console::colored_field{algebra::vector{5ul, 5ul}}},
//        window_container{console::window("rogue", 640, 480)}
//    });
//
//    manager.start_game();

    return 0;
}
