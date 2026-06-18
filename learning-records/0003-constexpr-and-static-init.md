# constexpr/const conversion done; hit and fixed the static-init-order trap

User replaced the `#define` constant wall in `scene_gameplay.cpp` with typed `constexpr`/`const`,
and converted `windowSize` to `constexpr Vector2` in `main.cpp`. Builds and runs.

**Concepts demonstrated:**
- Preprocessor (`#define`, text substitution before compile) vs compiler-level typed constants.
- `constexpr` ⊂ `const`: constexpr demands a compile-time-known value; const allows runtime init.
  Initially mis-stated const as "calculated before compilation"; corrected to "read-only, value may
  be computed at runtime." Accepted the correction.
- **Static initialization order fiasco, hit for real:** moved `INITIAL_PLAYER_POS`/`START_GRID_POS`
  to call `GetScreenWidth()` at file scope — globals initialize before `main()`, so `InitWindow`
  hadn't run and the calls returned 0 (player/grid mispositioned). Fixed by moving the
  runtime-dependent calculations *into* `InitPlayer`/`InitEnemies`, keeping only genuinely-constant
  values as globals. Strong design instinct (avoid hardcoded positions; derive from window size).

**Implications:** User now reliably reasons about *when* code runs (preprocess / static-init /
runtime), not just what it does — this unblocks teaching ctor/init ordering for the upcoming entity
classes (lesson 4). HiDPI item RESOLVED: on the user's 2x Retina, `GetScreenWidth()` returns the
*logical* 1280 (== requested windowSize), while `GetRenderWidth()` returns the *physical* 2560.
Using `GetScreenWidth` (logical) for gameplay coordinates is correct; raylib scales to the
framebuffer. User now has the logical-vs-physical pixel distinction. Next: lesson 3, range-for +
`std::array` to kill the `FOR_EACH_PROJECTILE`/`FOR_EACH_ENEMY` macros. See [[NOTES.md]].
