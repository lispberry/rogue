#ifndef ROGUE_VECTOR_HPP
#define ROGUE_VECTOR_HPP

#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <cstddef>
#include <tuple>
#include <cstddef>
#include <type_traits>
#include <array>
#include <cassert>
#include <typelist.hpp>

namespace rogue::algebra {
    using namespace rogue::typelist;

    template <size_t I, size_t N, typename U, typename ...T>
    struct copyT;

    template <size_t N, typename U, typename ...T>
    struct copyT<0, N, U, T...> {
        auto operator()(std::array<U, N> &to, const std::tuple<T...> &from) {}
    };

    template <size_t I, size_t N, typename U, typename ...T>
    struct copyT {
        auto operator()(std::array<U, N> &to, const std::tuple<T...> &from) {
            std::get<I>(to) = std::get<I>(from);
            copyT<I - 1, N, U, T...>{}(to, from);
        }
    };

    template<typename T, size_t Size>
    struct vector {
        std::array<T, Size> _data;
        using const_iterator = typename std::array<T, Size>::const_iterator;
        using iterator = typename std::array<T, Size>::iterator;

//        template <typename ...U>
//        constexpr explicit vector(U  ...z) {
//            static_assert(sizeT<type_list<U...>>::value == Size, "Wrong vector size");
//
//            copyT<Size - 1, Size, T, U...>{}(_data, std::make_tuple<U...>(z...));
//        }

        constexpr vector(std::initializer_list<T> data) {
            size_t i = 0;
            for (const auto &e : data) {
                _data[i++] = e;
            }
        }

        vector &operator=(const vector &) = default;
        vector &operator=(vector &&) noexcept = default;
        vector(vector &&) noexcept = default;
        vector(const vector &) = default;
        vector(vector &) = default;
        ~vector() = default;

        const T &operator()(size_t idx) const {
            return _data[idx];
        }

        T &operator()(size_t idx) {
            return _data[idx];
        }

        template <size_t Idx>
        const T &get() const {
            return std::get<Idx>(_data);
        }

        template <size_t Idx>
        T &get() {
            return std::get<Idx>(_data);
        }
        
        T &x() { return get<0>(); }
        T &y() { return get<1>(); }
        T &z() { return get<2>(); }
        T &w() { return get<3>(); }

        [[nodiscard]] const T &x() const { return get<0>(); }
        [[nodiscard]] const T &y() const { return get<1>(); }
        [[nodiscard]] const T &z() const { return get<2>(); }
        [[nodiscard]] const T &w() const { return get<3>(); }

        vector<T, Size> operator +(const vector<T, Size> &v) const {
            vector<T, Size> result{*this};
            result += v;
            return std::move(result);
        }

        vector<T, Size> operator -(const vector<T, Size> &v) const {
            vector<T, Size> result{*this};
            result -= v;
            return std::move(result);
        }

        // mutating members

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

//        template <typename Exp>
//        vector &operator=(const Exp &exp) {
//            size_t size = _data.size();
//            for (size_t i = 0; i < size; i++) {
//                _data[i] = exp[i];
//            }
//        }

        iterator begin() { return _data.begin(); }
        iterator end() { return _data.end(); }
        [[nodiscard]] const_iterator begin() const { return _data.begin(); }
        [[nodiscard]] const_iterator end() const { return _data.end(); }
    };
    
    template<typename T, size_t Size>
    vector<T, Size> scaled(vector<T, Size> self, const vector<T, Size> &scale) {
        for (size_t i = 0; i < Size; ++i) {
            self(i) *= scale(i);
        }
        return self;
    }

    template<class H, typename ...T>
    vector(H head, T ...tail) -> vector<H, 1 + sizeT<T...>::value>;

    struct expr {};

    template <typename Operand>
    auto subscript(const Operand &op, size_t idx) {
        if constexpr (std::is_base_of<expr, Operand>::value || std::is_array<Operand>::value) {
            return op[idx];
        } else {
            return op;
        }
    }

    template <typename Callable, typename ...Operands>
    class exp : expr {
    private:
        std::tuple<const Operands &...> _operands;
        Callable _apply;
    public:
        exp(Callable apply, const Operands &...operands) : _apply{apply}, _operands{operands...} {}

        auto operator[](size_t idx) {
            auto call_at_index = [=](const Operands & ...operands) {
                return _apply(subscript(operands, idx)...);
            };

            return std::apply(call_at_index, _operands);
        }
    };

    template <typename L, typename R>
    auto operator *(const L &x, const R &y) {
        return exp{[](auto const &x, auto const &y) { return x * y; }, x, y};
    }

    template <typename L, typename R>
    auto operator +(const L &x, const R &y) {
        return exp{[](auto const &x, auto const &y) { return x + y; }, x, y};
    }

    template<typename T> using vector2 = vector<T, 2>;
    template<typename T> using vector3 = vector<T, 3>;
    template<typename T> using vector4 = vector<T, 4>;

    using int2 = vector2<int>;
    using size_t2 = vector2<size_t>;

    template<typename T, size_t Size>
    std::string to_string(const vector<T, Size> &v) {
        std::vector<std::string> strings{};
        for (size_t i = 0; i < Size; ++i) strings.push_back(std::to_string(v(i)));
        return (boost::format("{%1%}") % boost::algorithm::join(strings, ", ")).str();
    }
}

#endif //ROGUE_VECTOR_HPP
