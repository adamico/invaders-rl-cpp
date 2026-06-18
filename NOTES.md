# Teaching Notes

## Learner profile
- Knows C well; "some C++" but shaky on modern idioms (RAII, smart pointers, move, templates).
- Goal is better game structure, not C++ theory for its own sake.

## Preferences
- **Refactor-driven**: each lesson must touch real code in this repo. No toy examples.
- **Keep it buildable**: game compiles & runs after every lesson.
- Caveman/terse comms in chat (global pref). Lessons themselves stay full and beautiful.

## Starting code state (2026-06-18)
- C-in-cpp: `GameState*` threaded through free functions; manual Load/Unload; macros for
  loops & constants. Files: main.cpp, scene_gameplay.cpp, scene_title.cpp, scene_gameover.cpp.

## Refactor roadmap (zone-of-proximal order)
1. [DONE L0001] RAII resources
2. [DONE L0002] constexpr instead of #define constants
3. [DONE L0003] range-for / std::array instead of FOR_EACH_* macros
4. [DONE L0004] const-correctness (draw path) — bridge step before methods
5. [DONE L0005] Entity classes — Projectile/Player/Enemy own update()/draw()/spawn; swarm logic
   stays free (C.4). Screen-size unified on windowSize constexpr.
6. [NEXT L0006] Scene base class + polymorphism instead of enum switch — introduces inheritance,
   virtual functions, virtual destructor, and likely unique_ptr + move semantics (deferred since L1).

## Deferred tidy-ups
- (resolved) Mixed screen-size source — unified on windowSize in L0005.
