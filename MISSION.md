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

## Out of scope (for now)
- The reinforcement-learning training loop (despite the repo name).
- Templates/metaprogramming beyond what the refactor genuinely needs.
- Switching build systems or graphics libraries — stays raylib + Makefile.
