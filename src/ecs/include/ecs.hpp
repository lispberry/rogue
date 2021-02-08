#ifndef ROGUE_ECS_HPP
#define ROGUE_ECS_HPP

#define GET_0(A, B) A
#define GET_1(A, B) B

namespace rogue::ecs {
    template <typename T>
    struct just {
        using type = T;
    };
    
    template <typename T>
    struct pointer {
        using type = T*;
    };
    
    template<typename ...T>
    struct typelist;
    
    template<size_t I, typename T>
    struct at;
    
    template<size_t I, typename Head, typename ...Tail>
    struct at<I, typelist<Head, Tail...>> {
        using value = typename at<I - 1, typelist<Tail...>>::value;
    };
    
    template<typename Head>
    struct at<0, typelist<Head>> {
        using value = Head;
    };
};

#define DEFINE_COMPONENT(NAME, FIELD_PAIR)                                 \
    template<template<typename _> typename F>                              \
    struct raw_##NAME {                                                    \
        typename F<GET_0 FIELD_PAIR>::type GET_1 FIELD_PAIR;               \
                                                                           \
        template<size_t i, typename T>                                                 \
        T get() {                                                          \
        }                                                                  \
                                                                           \
        template<typename T = at<0, field_types>::value>                                                        \
        T &get<0>() {                                       \
            return GET_1 FIELD_PAIR;                                       \
        }                                                                  \
        static constexpr size_t fields_number = 1;                         \
        using field_types = type_list<GET_0 FIELD_PAIR>;                   \
    };                                                                     \
    using NAME = raw_##NAME<rogue::ecs::just>;


/*
#include <tuple>
#include <type_traits>
#include <typelist.hpp>

using namespace rogue::typelist;

namespace rogue::ecs {
    template <typename ...Systems>
    class manager {

        template <typename System>
        manager<Systems..., System> add() {
            return {};
        }

        template <typename Entity>
        void push(const Entity &entity) {
            pushT<Systems, Entity>{}()
        }

        // System = type_list<type_list<Component>>
        // Entity = type_list<Component>
        // Component = ? type_list<T>

        template <typename System, typename Entity>
        struct belongs;

        template <typename System, typename Entity>
        struct belongs {
            static constexpr bool value = false;
        };

        template <typename ...Component>
        struct belongs<type_list<Component...>, type_list<Component...>> {
            static constexpr bool value = true;
        };
    };
}
*/
#endif //ROGUE_ECS_HPP
