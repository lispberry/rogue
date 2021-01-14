#ifndef ROGUE_ALGEBRA_HPP
#define ROGUE_ALGEBRA_HPP

#include <algebra/vector.hpp>

namespace rogue::algebra {
    template<typename T>
    struct field {
        T *const _pointer;
        const size_t2 _size;

        constexpr field(std::initializer_list<std::initializer_list<T>> list)
                : _size{list.begin()->size(), list.size()},
                  _pointer(new T[list.begin()->size() * list.size()]) {

            size_t i = 0;
            for (const auto &row : list) {
                assert(row.size() == _size.x());
                for (const auto &e : row) {
                    _pointer[i++] = e;
                }
            }
        }

        ~field() {
            delete[] _pointer;
        }


        // const members

        constexpr T &operator()(size_t x, size_t y) const {
            return _pointer[x + y * _size.x()];
        }

        const T &operator()(const size_t2 &v) const {
            return operator()(v.x(), v.y());
        }


        // mutating members

        inline T &operator()(size_t x, size_t y) {
            return _pointer[x + y * _size.x()];
        }

        inline T &operator()(size_t2 v) {
            return operator()(v.x(), v.y());
        }
    };
}

#endif //ROGUE_ALGEBRA_HPP
