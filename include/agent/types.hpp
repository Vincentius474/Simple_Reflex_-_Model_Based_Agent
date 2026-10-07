#pragma once
#include <format>
#include <string_view>

namespace agent {

    enum class Location : unsigned char { A, B };
    enum class Action : unsigned char { Suck, MoveLeft, MoveRight, NoOp };

    struct Percept {
        Location location;
        bool dirty;
    };

    constexpr std::string_view to_string(Location l) noexcept {
        return l == Location::A ? "A" : "B";
    }

    constexpr std::string_view to_string(Action a) noexcept {
        using enum Action;
        switch (a) {
            case Suck:
                return "SUCK";
            case MoveLeft:
                return "MOVE_LEFT";
            case MoveRight:
                return "MOVE_RIGHT";
            case NoOp:
                return "NO_OP";
        }
        return "?";
    }

}

// std::format integration
template <>
struct std::formatter<agent::Percept> {

    constexpr auto parse(std::format_parse_context& ctx) { 
        return ctx.begin();
    }

    auto format(const agent::Percept& p, std::format_context& ctx) const {
        return std::format_to(
            ctx.out(),
            "Percept{{loc={}, dirty={}}}", 
            to_string(p.location), p.dirty
        );
    }

};