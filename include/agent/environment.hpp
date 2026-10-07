#pragma once
#include "types.hpp"
#include <array>

namespace agent {

    class VacuumEnvironment {

        public:
            constexpr VacuumEnvironment(bool a_dirty, bool b_dirty, Location start) noexcept 
            : dirt_{ a_dirty, b_dirty }, agent_loc_{ start } {}

            [[nodiscard]] constexpr Percept getPercept() const noexcept {
                const auto i = index_of(agent_loc_);
                return { .location = agent_loc_, .dirty = dirt_[i] };
            }

            constexpr void applyAction(Action a) noexcept {
                using enum Action;
                const auto i = index_of(agent_loc_);
                switch (a) {
                    case Suck:      
                        dirt_[i] = false;         
                        break;
                    case MoveLeft:  
                        agent_loc_ = Location::A; 
                        break;
                    case MoveRight: 
                        agent_loc_ = Location::B; 
                        break;
                    case NoOp:                                
                        break;
                }
                ++steps_;
            }

            [[nodiscard]] constexpr bool isClean() const noexcept { return !dirt_[0] && !dirt_[1]; }
            [[nodiscard]] constexpr int steps() const noexcept { return steps_; }

        private:
            static constexpr std::size_t index_of(Location l) noexcept { return l == Location::A ? 0u : 1u; }
            std::array<bool, 2> dirt_;
            Location agent_loc_;
            int steps_ = 0;
            
    };

}