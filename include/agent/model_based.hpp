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
         * Update internal model from the current percept
         * Rule 1: Current room dirty -> suck 
         * Rule 2: the other room is not known clean -> go check it
         * Rule 3: both rooms known clean -> idle
        */
        public:
            [[nodiscard]] Action act(const Percept& p) noexcept {
                using enum Action;
                const auto i     = index_of(p.location);
                const auto other = 1u - i;

                world_[i] = p.dirty ? Cell::Dirty : Cell::Clean;

                if (p.dirty) 
                    return Suck;

                if (world_[other] != Cell::Clean)
                    return (p.location == Location::A) ? MoveRight : MoveLeft;

                return NoOp;
            }

            // Introspection for tests or UI
            [[nodiscard]] constexpr bool known_dirty(std::size_t i) const noexcept {
                return world_[i] == Cell::Dirty;
            }
            [[nodiscard]] constexpr bool known_clean(std::size_t i) const noexcept {
                return world_[i] == Cell::Clean;
            }

            constexpr void reset() noexcept {
                world_ = { Cell::Unknown, Cell::Unknown };
            }

        private:
            enum class Cell : unsigned char { Unknown, Dirty, Clean };

            static constexpr std::size_t index_of(Location l) noexcept {
                return l == Location::A ? 0u : 1u;
            }

            std::array<Cell, 2> world_{ Cell::Unknown, Cell::Unknown };
    };

}