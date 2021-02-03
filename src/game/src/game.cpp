#include <console.hpp>
#include <algebra.hpp>
#include <ecs.hpp>
#include <boost/hana.hpp>

using namespace rogue;

DEFINE_COMPONENT(
    positioned,
    (algebra::size_t2, position)
)



template<int I>
struct _reference_synchronize {
    void operator()(raw_positioned<ecs::just> &from, raw_positioned<ecs::pointer> &to) {
        to.get<I>() = &from.get<I>();
        _reference_synchronize<I - 1>{}(from, to);
    }
};

template<>
struct _reference_synchronize<-1> {
    void operator()(const raw_positioned<ecs::just> &from, raw_positioned<ecs::pointer> &to) {}
};

//template<template<template<typename __> typename _> typename T>
auto reference_replace(raw_positioned<ecs::just> &object) {
    raw_positioned<ecs::pointer> result{};
    _reference_synchronize<raw_positioned<ecs::just>::fields_number>{}(object, result);
    return result;
}

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
//    console::sdl sdl{};
//
//    console::window window("rogue", 640, 480);
//    console::render_field(
//        window,
//        console::colored("Hello, world!\nTest line", console::color::black, console::color::white));
//    SDL_Delay(5000);

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
    
    std::tuple a{1, 2, "1"};
    
    std::get<1>(a);

    raw_positioned<ecs::just> p{algebra::size_t2{0ul, 1ul}};
    
    auto f = reference_replace(p);
    f.position->x() += 1;
    
    std::cout << algebra::to_string(p.position);

    return 0;
}
