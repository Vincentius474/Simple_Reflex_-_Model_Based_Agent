#include "agent/types.hpp"
#include "agent/concepts.hpp"
#include "agent/environment.hpp"
#include "agent/simple_reflex.hpp"
#include "agent/model_based.hpp"

#include <format>
#include <iostream>
#include <random>
#include <ranges>
#include <string_view>

using namespace agent;

struct RunResult {
    std::string_view name;
    bool clean;
    int steps;
};

// Generic simulator: works with any type satisfying `agent::Agent`.
template <Agent A>
RunResult run_once(std::string_view name, A& a, bool a_dirty, bool b_dirty, 
                    Location start, int max_steps = 50)
{
    VacuumEnvironment env{ a_dirty, b_dirty, start };
    int steps = 0;
    for (; steps < max_steps && !env.isClean(); ++steps) {
        env.applyAction(a.act(env.getPercept()));
    }

    return { name, env.isClean(), steps };
}

int main() {

    static_assert(Agent<SimpleReflex>);
    static_assert(Agent<ModelBased>);
    static_assert(ResettableAgent<ModelBased>);

    std::mt19937 rng{ std::random_device{}() };
    std::bernoulli_distribution coin{ 0.5 };
    std::uniform_int_distribution<int> pick{ 0, 1 };

    std::cout << std::format("{:=<55}\n", "");
    std::cout << " Agent Benchmark — Simple Reflex vs Model-Based\n";
    std::cout << std::format("{:=<55}\n", "");

    int simple_wins = 0, model_wins = 0, ties = 0;

    for (int trial : std::views::iota(1, 11)) {
        const bool da = coin(rng);
        const bool db = coin(rng);
        const Location st = pick(rng) ? Location::A : Location::B;

        SimpleReflex sra;
        ModelBased   mba;

        auto r1 = run_once("Simple", sra, da, db, st);
        auto r2 = run_once("Model",  mba, da, db, st);

        std::cout << std::format("\nTrial {:>2}  [A:{}  B:{}  start:{}]\n", trial,
                    da ? "dirty" : "clean", db ? "dirty" : "clean", to_string(st));

        std::cout << std::format("  Simple Reflex : {:>2} steps {}\n",
                                 r1.steps, r1.clean ? "" : "(INCOMPLETE)");

        std::cout << std::format("  Model-Based   : {:>2} steps {}\n",
                                 r2.steps, r2.clean ? "" : "(INCOMPLETE)");

        if (!r1.clean && r2.clean)  
            ++model_wins;
        else if (r1.clean && !r2.clean)  
            ++simple_wins;
        else if (r1.steps < r2.steps)    
            ++simple_wins;
        else if (r2.steps < r1.steps)    
            ++model_wins;
        else                            
            ++ties;
    }

    std::cout << std::format("\n{:=<55}\n", "");
    std::cout << std::format(" Summary: Simple wins={}   Model wins={}   Ties={}\n",
                            simple_wins, model_wins, ties);
    std::cout << std::format("{:=<55}\n", "");

    return 0;

}