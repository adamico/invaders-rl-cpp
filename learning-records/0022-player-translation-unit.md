# Promote Player to its own translation unit (physical design / header hygiene)

Not a numbered HTML lesson — a mechanical refactor with real physical-design content, done inline.
Motivated by the user's question "is it time to promote Player into its own header file?" after L20/L21
grew Player into the most behaviour-heavy entity. Yes: it was the odd-one-out (Swarm/ProjectilePool/Pool
each had their own .h/.cpp, but Player was declared in game.h with its methods squatting in
scene_gameplay.cpp).

**What moved:**
- `include/player.h` — self-contained (raylib only), holds `class Player` + its owned constants
  (PLAYER_RADIUS/SPEED/HEALTH).
- `src/player.cpp` — all `Player::*` definitions, lifted out of scene_gameplay.cpp.
- `include/canvas.h` — NEW tiny header for shared render-layout constants (CANVAS_SIZE/CANVAS_OFFSET),
  included by both player.cpp and scene_gameplay.cpp.
- `game.h` is now Player-free. Makefile globs src/*.cpp so player.cpp is picked up with no build change.

**Concepts (physical design, not language features):**
- A header is a DEPENDENCY SURFACE. Putting CANVAS_OFFSET in the kitchen-sink game.h would force player.cpp
  to `#include "resources.h"` just to draw — coupling Player to Enemy/Projectile/GameResources it doesn't use.
  A focused canvas.h keeps Player's deps = raylib + canvas.h only. (This is the real reason entity-per-file
  matters: controlling who-depends-on-what, not tidiness.)
- Owner-owns-its-constants: PLAYER_* live in player.h (travel with Player if it's ever reused); shared
  layout constants live in canvas.h; neither in game.h.
- constexpr at namespace scope = INTERNAL LINKAGE -> safe to define in a header (each TU its own copy, no
  ODR violation, no `inline`/`extern` needed). Contrast a non-const global (needs extern + one .cpp def).
- Header = the promise (declarations); .cpp = the fulfilment (one definition, compiled once). Splitting
  means editing Player no longer recompiles every file that includes the scene.

**Design win folded in (user took the "right" option, not the lazy one):** scene_gameplay was computing the
player's spawn position using PLAYER_RADIUS (line 40) — Player's concern leaking into the scene. Instead of
just moving the constant, made `Player::reset()` take NO argument and compute its own spawn from screen size
+ its own radius. Result: PLAYER_RADIUS has exactly one owner (player.h), the scene's spawn-math disappeared,
initGameplay shrank to `state->player.reset()`. Tell-Don't-Ask again: don't ask Player its radius to place
it; tell it to reset and let it place itself.

**isocpp Core Guidelines basis (user asked):** SF.11 (headers self-contained), SF.2 (no non-inline object/
function defs in headers — but namespace-scope constexpr is fine), SF.10 (avoid implicit transitive
includes), SF.1/5/8. The deeper coupling/levelization instinct is Lakos "Large-Scale C++ Software Design"
rather than a single CG rule. (Caveat noted to user: verify exact SF numbers at isocpp.)

**Status:** builds clean, plays identical. Player is now a proper standalone TU; dependency surface
minimized. Scope held to Player only — Enemy/Projectile/ProjectileSpec/GameResources stay in game.h until
THEY grow rules (YAGNI). Next: L21 (lives+respawn) now lands cleanly in player.h/player.cpp. See [[NOTES.md]].
