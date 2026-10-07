#include "agent/types.hpp"
#include "agent/concepts.hpp"
#include "agent/environment.hpp"
#include "agent/simple_reflex.hpp"
#include "agent/model_based.hpp"

#include <iostream>
#include <source_location>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <functional>

using namespace agent;

// Compile time test (otherwise the program won't compile if these fail)
namespace compile_time_tests {

    consteval bool simple_reflex_rules_hold() {
        using enum Action;
        const SimpleReflex a;
        return a.act({ Location::A, true  }) == Suck
            && a.act({ Location::B, true  }) == Suck
            && a.act({ Location::A, false }) == MoveRight
            && a.act({ Location::B, false }) == MoveLeft;
    }

    consteval bool environment_suck_removes_dirt() {
        VacuumEnvironment env{ true, false, Location::A };
        if (!env.getPercept().dirty) 
            return false;
        env.applyAction(Action::Suck);
        return !env.getPercept().dirty;
    }

    consteval bool environment_moves_change_location() {
        VacuumEnvironment env{ false, false, Location::A };
        env.applyAction(Action::MoveRight);
        if (env.getPercept().location != Location::B) 
            return false;
        env.applyAction(Action::MoveLeft);
        return env.getPercept().location == Location::A;
    }

    static_assert(simple_reflex_rules_hold());
    static_assert(environment_suck_removes_dirt());
    static_assert(environment_moves_change_location());

}

// Runtime test - tiny framework no external dependencies
struct Test { std::string_view name; std::function<void()> fn; };

std::vector<Test>& registry() { static std::vector<Test> r; return r; }

struct Registrar {
    Registrar(std::string_view n, std::function<void()> f) {
        registry().push_back({ n, std::move(f) });
    }
};

#define TEST(name) \
    static void name(); \
    static Registrar reg_##name(#name, name); \
    static void name()

#define CHECK(cond) do { \
    if (!(cond)) { \
        const auto loc = std::source_location::current(); \
        std::cerr << "    FAIL: " #cond "  at " \
                  << loc.file_name() << ':' << loc.line() << '\n'; \
        throw std::runtime_error("assertion failed"); \
    } \
} while (0)

#define CHECK_EQ(a, b) do { \
    auto _va = (a); auto _vb = (b); \
    if (!(_va == _vb)) { \
        const auto loc = std::source_location::current(); \
        std::cerr << "    FAIL: " #a " == " #b "  at " \
                  << loc.file_name() << ':' << loc.line() << '\n'; \
        throw std::runtime_error("assertion failed"); \
    } \
} while (0)

// Simple Reflex 
#include <tuple>   // for std::ignore

TEST(SimpleReflex_Is_Stateless) {
    SimpleReflex a;
    auto r1 = a.act({ Location::A, false });
    std::ignore = a.act({ Location::B, true });
    std::ignore = a.act({ Location::A, true });
    CHECK_EQ(r1, a.act({ Location::A, false }));
}

// Model Based
TEST(ModelBased_RemembersCleanedRoom) {
    ModelBased a;
    std::ignore = a.act({ Location::A, true  });
    std::ignore = a.act({ Location::A, false });
    CHECK(a.known_clean(0));
    CHECK(!a.known_dirty(0));
}

TEST(ModelBased_MovesTowardKnownDirty) {
    ModelBased a;
    std::ignore = a.act({ Location::A, true  });   // A dirty, suck
    std::ignore = a.act({ Location::A, false });   // A clean; B still unknown
    // Standing in A, B unknown -> go check B
    CHECK(a.act({ Location::A, false }) == Action::MoveRight);
}

TEST(ModelBased_IdlesWhenDone) {
    ModelBased a;
    std::ignore = a.act({ Location::A, false });   // A clean; B unknown -> move
    std::ignore = a.act({ Location::B, false });   // B clean now
    // Both known clean -> idle
    CHECK(a.act({ Location::B, false }) == Action::NoOp);
}

TEST(ModelBased_Reset_ClearsModel) {
    ModelBased a;
    std::ignore = a.act({ Location::A, true });
    a.reset();
    CHECK(!a.known_dirty(0));
    CHECK(!a.known_clean(0));
}


// Integration
template <Agent A>
bool cleans_both_rooms(A&& a, Location start, int max_steps = 20) {
    VacuumEnvironment env{ true, true, start };
    for (int i = 0; i < max_steps && !env.isClean(); ++i)
        env.applyAction(a.act(env.getPercept()));
    return env.isClean();
}

TEST(Integration_All_Agents_CleanFromBoth) {
    CHECK(cleans_both_rooms(SimpleReflex{}, Location::A));
    CHECK(cleans_both_rooms(SimpleReflex{}, Location::B));
    CHECK(cleans_both_rooms(ModelBased{}, Location::A));
    CHECK(cleans_both_rooms(ModelBased{}, Location::B));
}

// Runner
int main() {

    int passed = 0, failed = 0;
    for (auto& t : registry()) {
        std::cout << "[ RUN  ] " << t.name << '\n';
        try {
            t.fn();
            std::cout << "[  OK  ] " << t.name << '\n';
            ++passed;
        } catch (const std::exception& e) {
            std::cout << "[ FAIL ] " << t.name << " — " << e.what() << '\n';
            ++failed;
        }
    }

    std::cout << "\n========================================\n";
    std::cout << " Passed: " << passed << "   Failed: " << failed << "\n";
    std::cout << "========================================\n";
    return failed == 0 ? 0 : 1;

}