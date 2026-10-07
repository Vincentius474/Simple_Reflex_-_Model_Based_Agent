#pragma once
#include "types.hpp"
#include <concepts>

namespace agent {

    template <typename T>
    concept Agent = requires(T t, const Percept& p) {
        { t.act(p) } -> std::same_as<Action>;
    };

    template <typename T>
    concept ResettableAgent = Agent<T> && requires(T t) { t.reset(); };

    template <typename T>
    concept EnvironmentLike = requires(T env) {

        { env.getPercept() } -> std::same_as<Percept>;
        { env.isClean()    } -> std::same_as<bool>;
        { env.steps()      } -> std::same_as<int>;
        env.applyAction(Action::NoOp);

    };

} 