# Mission: Port Invaders from C-style to idiomatic C++

## Why
The game currently works but is written as C inside `.cpp` files: free functions passing
`GameState*` everywhere, manual resource Load/Unload, and macros standing in for language
features. The goal is a **better-structured game** — using real C++ idioms to make the code
safer, clearer, and easier to extend (new scenes, enemy types, weapons) without rewriting it.

## Success looks like
- Resources load/unload themselves via RAII — no manual `UnloadGameResources` call to forget.
- Game entities (Player, Enemy, Projectile) are classes that own their own update/draw logic.
- Scenes are an extensible type hierarchy, not a `switch` on an enum.
- Macros (`FOR_EACH_*`, geometry constants) replaced by `constexpr`, range-for, and containers.
- The user can read the code and explain *why* each C++ idiom beats the C version it replaced.

## Constraints
- User knows some C++ but is shaky on modern idioms (RAII, smart pointers, move, templates).
- Refactor-driven pacing: every lesson refactors one real piece of *this* game, learn-by-doing.
- The game must still build and run after each lesson (no big-bang rewrites).

## Phase 2: finish the game
The C->C++ idiomatic refactor is complete (17 lessons). The codebase is now a clean base
to build ON. Next goal: make Space Invaders a *complete* game, not just a clean skeleton —
using the idioms already learned (entities, pools, scenes, RAII) to add missing gameplay.
Refactor-driven still: one real feature at a time, game stays buildable after each.

Candidate features:
- Enemies shoot back (the `fire(dir)` hook already exists, currently unused).
- Defensive bunkers/shields the player hides behind.
- Real lives (currently enemy contact = instant death despite health = 5).
- Waves / levels: clear the swarm -> next wave, faster + lower.
- Polish: high-score persistence, screen shake, particle explosions.

## Out of scope
- "RL" in the repo name = **raylib**, NOT reinforcement learning. No ML/agent/training loop.
- Switching build systems or graphics libraries — stays raylib + Makefile.
