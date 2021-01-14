#ifndef ROGUE_ALGEBRA_HPP
#define ROGUE_ALGEBRA_HPP

#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

namespace rogue::algebra {
    template<typename T, size_t Size>
    struct vector {
        T *const _pointer;

        constexpr vector(std::initializer_list<T> list) : _pointer(new T[Size]) {
            assert(list.size() == Size);
            size_t i = 0;
            for (const auto &e : list) _pointer[i++] = e;
        }

        ~vector() {
            delete[] _pointer;
        }


        // const members

        inline const T &operator()(size_t i) const {
            return _pointer[i];
        }

        inline const T &x() const { return operator()(0); }
        inline const T &y() const { return operator()(1); }
        inline const T &z() const { return operator()(2); }
        inline const T &w() const { return operator()(3); }

        vector<T, Size> operator +(const vector<T, Size> &v) const {
            vector<T, Size> result{this};
            result += v;
            return std::move(result);
        }

        vector<T, Size> operator -(const vector<T, Size> &v) const {
            vector<T, Size> result{this};
            result -= v;
            return std::move(result);
        }


        // mutating members

        inline T &operator()(size_t i) {
            return _pointer[i];
        }

        inline T &x() { return operator()(0); }
        inline T &y() { return operator()(1); }
        inline T &z() { return operator()(2); }
        inline T &w() { return operator()(3); }

        vector<T, Size> &operator +=(vector<T, Size> v) {
            for (size_t i = 0; i < Size; ++i) {
                operator()(i) += v(i);
            }
            return this;
        }

        vector<T, Size> &operator -=(vector<T, Size> v) {
            for (size_t i = 0; i < Size; ++i) {
                operator()(i) -= v(i);
            }
            return this;
        }

        T *begin() { return _pointer; }

        T *end() { return _pointer + Size; }
    };

    template<typename T, size_t Size>
    std::string to_string(const vector<T, Size> &v) {
        std::vector<std::string> strings{};
        for (size_t i = 0; i < Size; ++i) strings.push_back(std::to_string(v._pointer[i]));
        return (boost::format("{%1%}") % boost::algorithm::join(strings, ", ")).str();
    }

    template<typename T> using vector2 = vector<T, 2>;
    template<typename T> using vector3 = vector<T, 3>;
    template<typename T> using vector4 = vector<T, 4>;

    using int2 = vector2<int>;
    using size_t2 = vector2<size_t>;


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
