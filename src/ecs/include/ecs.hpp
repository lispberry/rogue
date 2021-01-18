#ifndef ROGUE_ECS_HPP
#define ROGUE_ECS_HPP

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

#endif //ROGUE_ECS_HPP
