#pragma once
#include "types.hpp"
#include <array>

namespace agent {

    /** 
     * Keeps an internal model of the world so it can succeed where the 
     * simple reflex agent loops (partially obesrvable environments).
    */
    class ModelBased {

        /** 
         * Update the internal model from the current percept
         * Rule 1: If the current room is dirty then suck
         * Rule 2: If the other room is known to be dirty and not known to be clean then go
         * Rule 3: Otherwise there is nothing useful to do
        */
        public:
            [[nodiscard]] Action act(const Percept& p) noexcept {
                using enum Action;
                const auto i     = index_of(p.location);
                const auto other = 1u - i;

                if (p.dirty) 
                    known_dirty_[i] = true;
                else
                    known_clean_[i] = true;

                if (p.dirty) return Suck;

                if (known_dirty_[other] && !known_clean_[other]) {
                    return (p.location == Location::A) ? MoveRight : MoveLeft;
                }

                return NoOp;
            }

            [[nodiscard]] constexpr bool known_dirty(std::size_t i) const noexcept { return known_dirty_[i]; }
            [[nodiscard]] constexpr bool known_clean(std::size_t i) const noexcept { return known_clean_[i]; }

            constexpr void reset() noexcept {
                known_dirty_ = { false, false };
                known_clean_ = { false, false };
            }

        private:
            static constexpr std::size_t index_of(Location l) noexcept { 
                return l == Location::A ? 0u : 1u;
            }
            std::array<bool, 2> known_dirty_{};
            std::array<bool, 2> known_clean_{};

    };

}