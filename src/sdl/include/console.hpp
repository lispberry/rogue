#ifndef ROGUE_CONSOLE_HPP
#define ROGUE_CONSOLE_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <boost/algorithm/string.hpp>
#include <utility>
#include <algebra.hpp>

namespace rogue::console {
    namespace internal {
        template<typename T>
        T *logged(T *subject, const std::string &message, std::ostream &stream = std::cout) {
            if (subject == nullptr)
                stream << message << "\n" << SDL_GetError() << "\n";
            return subject;
        }

        inline int logged(int subject, const std::string &message, std::ostream &stream = std::cout) {
            if (subject != 0)
                stream << message << "\n" << SDL_GetError() << "\n";
            return subject;
        }

        inline void render_texture(SDL_Texture *tex, SDL_Renderer *ren, algebra::size_t2 position) {
            SDL_Rect dst;
            dst.x = position.x();
            dst.y = position.y();
            SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
            SDL_RenderCopy(ren, tex, nullptr, &dst);
        }
    }

    inline auto make_font(const std::string &path, int size) {
        return internal::logged(
            TTF_OpenFont(path.c_str(), size),
            "Can not open fonts at " + path
        );
    }

    inline constexpr SDL_Color make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        return {r, g, b, a};
    }

    namespace color {
        constexpr SDL_Color
            white = make_color(255, 255, 255),
            black = make_color(0, 0, 0),
            transparent = make_color(0, 0, 0, 0);
    }

    struct ColoredChar {
        uint16_t character;
        SDL_Color foreground, background;

        ColoredChar()= default;

        constexpr ColoredChar(uint16_t character, SDL_Color fg, SDL_Color bg)
            : character(character), foreground(fg), background(bg) {}
    };



    inline algebra::field<ColoredChar> colored(
        const std::string &text,
        SDL_Color foreground = color::white,
        SDL_Color background = color::black,
        ColoredChar default_character = ColoredChar{static_cast<uint16_t>(' '), color::transparent, color::transparent}
    ) {
        std::vector<std::string> lines;
        boost::split(lines, text, boost::is_any_of("\n"));

        std::size_t width = 0;
        for (const auto &line : lines) {
            if (line.size() > width) width = line.size();
        }

        algebra::field<ColoredChar> result{algebra::size_t2{width, lines.size()}};

        algebra::size_t2 v{0, 0};
        for (const auto &line : lines) {
            v.x() = 0;
            for (const auto &c : line) {
                result(v) = ColoredChar{static_cast<uint16_t>(c), foreground, background};

                v.x()++;
            }

            for (; v.x() < width; v.x()++) result(v) = default_character;
            v.y()++;
        }

        return result;
    }

    struct Window {
        TTF_Font *_font;
        algebra::size_t2 _glyph_size{0, 0};

        SDL_Window *_window;
        SDL_Renderer *_renderer;

        inline Window(const std::string &title, int w, int h) {
            internal::logged(SDL_Init(SDL_INIT_EVERYTHING), "Can not initialize SDL");

            _window = internal::logged(
                SDL_CreateWindow(
                    title.c_str(),
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    w, h,
                    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                ),
                "Can not initialize window"
            );

            _renderer = internal::logged(
                SDL_CreateRenderer(
                    _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                ),
                "Can not initialize renderer"
            );

            internal::logged(TTF_Init(), "Can not initialize TTF");
            _font = make_font("/home/lispberry/projects/rogue/assets/fonts/consolas.ttf", 14);

            auto example_glyph = TTF_RenderGlyph_Shaded(_font, 21, make_color(255, 255, 255), make_color(0, 0, 0));
            _glyph_size = algebra::size_t2{static_cast<size_t>(example_glyph->w), static_cast<size_t>(example_glyph->h)};
            delete example_glyph;
        }

        ~Window() {
            SDL_DestroyWindow(_window);
            SDL_Quit();
        }
    };

    inline void render_matrix(const Window &window, const algebra::field<ColoredChar> &field) {
        SDL_RenderClear(window._renderer);

        algebra::size_t2 v{0, 0};
        for (; v.x() < field.size().x(); ++v.x()) {
            for (v.y() = 0; v.y() < field.size().y(); ++v.y()) {
                auto surface = internal::logged(
                    TTF_RenderGlyph_Shaded(
                        window._font,
                        field(v).character,
                        field(v).foreground,
                        field(v).background
                    ),
                    "Can't create surface of character " + std::to_string(field(v).character) + "'"
                );

                auto texture = internal::logged(
                    SDL_CreateTextureFromSurface(window._renderer, surface),
                    "Can't create texture"
                );

                internal::render_texture(
                    texture,
                    window._renderer,
                    window._glyph_size.scaled(v)
                );

                delete surface;
            }
        }

        SDL_RenderPresent(window._renderer);
    }
}

#endif //ROGUE_CONSOLE_HPP
