#ifndef ROGUE_TYPELIST_HPP
#define ROGUE_TYPELIST_HPP

#include <type_traits>

namespace rogue::typelist {
    template <typename ...List>
    struct type_list {};

    template <typename List>
    struct sizeT;

    template <>
    struct sizeT<> {
        constexpr static int value = 0;
    };

    template <typename H, typename ...T>
    struct sizeT<type_list<H, T...>> {
        constexpr static int value = 1 + sizeT<T...>::value;
    };

    template <typename List>
    using size_v = size_t<List>::value;

    template <typename List>
    struct headT;

    template <typename Head, typename ...Tail>
    struct headT<type_list<Head, Tail...>> {
        using type = Head;
    };

    template <typename List>
    using head = typename headT<List>::type;

    template <typename List>
    struct tailT;

    template <typename Head, typename ...Tail>
    struct tailT<type_list<Head, Tail...>> {
        using type = type_list<Tail...>;
    };

    template <typename List>
    using tail = typename tailT<List>::type;

    template <typename Val, typename List>
    struct consT;

    template <typename Val, typename ...Tail>
    struct consT<Val, type_list<Tail...>> {
        using type = type_list<Val, Tail...>;
    };

    template <typename Val, typename List>
    using cons = typename consT<Val, List>::type;

    template <typename List, template<typename V> class Pred>
    struct filterT;

    template <typename Head, typename ...Tail, template<typename V> class Pred>
    struct filterT<type_list<Head, Tail...>, Pred> {
        using type = std::conditional<Pred<Head>::value,
                cons<Head, typename filterT<type_list<Tail...>, Pred>::type>,
                typename filterT<type_list<Tail...>, Pred>::type>;
    };

    template <typename List, template<typename V> class Pred>
    using filter = typename filterT<List, Pred>::type;

    template <typename List, template<typename T> class Fn>
    struct mapT;

    template <typename Head, template<typename T> class Fn>
    struct mapT<type_list<Head>, Fn> {
        using type = type_list<typename Fn<Head>::type>;
    };

    template <typename Head, typename ...Tail, template<typename T> class Fn>
    struct mapT<type_list<Head, Tail...>, Fn> {
        using type = cons<typename Fn<Head>::type, typename mapT<type_list<Tail...>, Fn>::type>;
    };

    template <typename List, template<typename T> class Fn>
    using map = typename mapT<List, Fn>::type;

    template <typename List1, typename List2>
    struct appendT;

    template <typename Head, typename ...Tail, typename List2>
    struct appendT<type_list<Head, Tail...>, List2> {
        using type = cons<Head, typename appendT<type_list<Tail...>, List2>::type>;
    };

    template <typename List2>
    struct appendT<type_list<>, List2> {
        using type = List2;
    };

    template <typename L1, typename L2>
    using append = typename appendT<L1, L2>::type;

    template <typename List>
    struct flattenT;

    template <>
    struct flattenT<type_list<>> {
        using type = type_list<>;
    };

    template <typename List, typename ...Tail>
    struct flattenT<type_list<List, Tail...>> {
        using type = append<List, typename flattenT<type_list<Tail...>>::type>;
    };

    template <typename List>
    using flatten = typename flattenT<List>::type;

    template <typename List, template<typename T> class Fn>
    struct flatmapT {
        using type = flatten<map<List, Fn>>;
    };

    template <typename List, template<typename T> class Fn>
    using flatmap = typename flatmapT<List, Fn>::type;
}

#endif //ROGUE_TYPELIST_HPP
