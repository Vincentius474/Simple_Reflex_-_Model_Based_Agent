# AI Agents in Modern C++20

Two classic agent architectures from Russell & Norvig's *Artificial
Intelligence: A Modern Approach*, implemented idiomatically in C++20.

| Agent | Memory | Behaviour |
|---|---|---|
| **Simple Reflex** | none | Acts on the current percept only (`IF dirty THEN suck`) |
| **Model-Based**   | world model | Remembers which rooms it has cleaned |

The domain is the textbook **vacuum-cleaner world**: two rooms, dirt
appears randomly, the agent must clean both.

## Highlights

- **Compile-time verified** — the Simple Reflex agent and environment
  are validated with `consteval` + `static_assert`; failures break the build.
- **Concept-based design** — `Agent`, `ResettableAgent`, and
  `EnvironmentLike` concepts; a single templated simulator drives both agents.
- **Modern C++20** — `std::format`, `std::source_location`, ranges,
  designated initializers, `using enum`.
- **Zero dependencies** — no external test framework; the whole test
  harness is ~60 lines of standard library.

## Build & run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure   # unit tests
./build/vacuum_agent                         # benchmark
```

### Sample output

```
=======================================================
 Agent Benchmark - Simple Reflex vs Model-Based
=======================================================

Trial  1  [A:dirty  B:clean  start:B]
  Simple Reflex :  3 steps
  Model-Based   :  3 steps
...
=======================================================
 Summary: Simple wins=4   Model wins=2   Ties=4
=======================================================
```

## Why this project exists

A **simple reflex agent** is the smallest non-trivial agent architecture.
It works beautifully on fully observable environments, but *cannot
remember anything* — so it loops forever on partially observable ones.
The **model-based agent** fixes that by maintaining internal state.

This repo shows both, side by side, in clean, testable C++20.

## License
MIT