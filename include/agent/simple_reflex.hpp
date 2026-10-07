#pragma once
#include "types.hpp"

namespace agent {

    /**
     * Stateless agent: decisions depend only on the current percept
     * Rule 1: If dirty then suck
     * Rule 2: If clean and in A then move to the right
     * Rule 3: If clean and in B then move to the left
    **/
    class SimpleReflex {
        public:
            [[nodiscard]] constexpr Action act(const Percept& p) const noexcept {
                using enum Action;
                if (p.dirty) return Suck;
                if (p.location == Location::A) return MoveRight;
                return MoveLeft;
            }
    };

}