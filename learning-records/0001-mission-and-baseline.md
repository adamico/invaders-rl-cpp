# Mission set: better-structured game via idiomatic C++; baseline established

User wants the game restructured using real C++ idioms (not RL prep, not pure C++ theory).
Self-rates "some C++" but shaky on modern idioms (RAII, smart pointers, move, templates), and
chose **refactor-driven** pacing — every lesson must change real code in this repo and keep it
buildable.

Baseline code is C-in-cpp: `GameState*` threaded through free functions, manual Load/Unload,
`#define` constants, `FOR_EACH_*` loop macros. Roadmap recorded in [[NOTES.md]]:
RAII → constexpr → range-for/std::array → entity classes → scene polymorphism.

Lesson 0001 (RAII for `GameResources`) delivered but **not yet confirmed applied/understood** —
write a follow-up record once the user builds it and answers the quiz, per the "no completion
before evidence" rule.
