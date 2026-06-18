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
5. [NEXT L0005] Entity classes — start with Projectile (smallest), then Enemy, Player.
   Each owns update()/draw(); draw() is const. Tracer-bullet one entity at a time, keep buildable.
6. Scene base class + polymorphism instead of enum switch

## Deferred tidy-ups
- Mixed screen-size source: Update* use `windowSize` global, Init* use GetScreenWidth(). Reconcile
  during entity refactor.
