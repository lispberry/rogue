#ifndef ROGUE_CONSOLE_HPP
#define ROGUE_CONSOLE_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <boost/algorithm/string.hpp>
#include <utility>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

using boost::numeric::ublas::matrix;
using boost::numeric::ublas::vector;

namespace console {
    namespace internal {
        template<typename T>
        T *logged(T *subject, const std::string &message, std::ostream &stream = std::cout) {
            if (subject == nullptr)
                stream << message << "\n" << SDL_GetError() << "\n";
            return subject;
        }

        int logged(int subject, const std::string &message, std::ostream &stream = std::cout) {
            if (subject != 0)
                stream << message << "\n" << SDL_GetError() << "\n";
            return subject;
        }

        void render_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
            SDL_Rect dst;
            dst.x = x;
            dst.y = y;
            SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
            SDL_RenderCopy(ren, tex, nullptr, &dst);
        }
    }

    auto make_font(const std::string &path, int size) {
        return internal::logged(
                TTF_OpenFont(path.c_str(), size),
                "Can not open font at " + path
        );
    }

    constexpr SDL_Color make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
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



    matrix<ColoredChar> colored(
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

        matrix<ColoredChar> result{width, lines.size()};

        int x, y = 0;
        for (const auto &line : lines) {
            x = 0;
            for (const auto &c : line) {
                result(x, y) = ColoredChar{static_cast<uint16_t>(c), foreground, background};

                x++;
            }

            for (; x < width; x++) result(x, y) = default_character;
            y++;
        }

        return result;
    }

    struct Window {
        TTF_Font *_font;
        vector<int> _glyph_size{2};

        SDL_Window *_window;
        SDL_Renderer *_renderer;

        Window(const std::string &title, int w, int h) {
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
            _font = make_font("/home/nik/Projects/RogueSdl/assets/fonts/consolas.ttf", 14);

            auto example_glyph = TTF_RenderGlyph_Shaded(_font, 21, make_color(255, 255, 255), make_color(0, 0, 0));
            _glyph_size(0) = example_glyph->w;
            _glyph_size(1) = example_glyph->h;
            delete example_glyph;
        }

        ~Window() {
            SDL_DestroyWindow(_window);
            SDL_Quit();
        }
    };

    void render_matrix(const Window &window, const matrix<ColoredChar> &matrix) {
        SDL_RenderClear(window._renderer);

        for (int x = 0; x < matrix.size1(); ++x) {
            for (int y = 0; y < matrix.size2(); ++y) {
                auto surface = internal::logged(
                        TTF_RenderGlyph_Shaded(
                                window._font,
                                matrix(x, y).character,
                                matrix(x, y).foreground,
                                matrix(x, y).background
                        ),
                        "Can't create surface of character " + std::to_string(matrix(x, y).character) + "'"
                );

                auto texture = internal::logged(
                        SDL_CreateTextureFromSurface(window._renderer, surface),
                        "Can't create texture"
                );

                internal::render_texture(
                        texture,
                        window._renderer,
                        x * window._glyph_size(0),
                        y * window._glyph_size(1)
                );

                delete surface;
            }
        }

        SDL_RenderPresent(window._renderer);
    }
}

#endif //ROGUE_CONSOLE_HPP
