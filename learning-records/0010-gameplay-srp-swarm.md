# SRP: extracted Swarm class out of the gameplay god-file

User completed lesson 0009. Diagnosed the responsibilities tangled in `scene_gameplay.cpp` and lifted
exactly one: the enemy swarm. Created `include/swarm.h` + `src/swarm.cpp` with a `Swarm` struct owning
`std::array<Enemy> enemies`, `direction`, `speed`, `activeCount` and methods `reset()` / `update(dt)` /
`draw()`. `GameState` shrank — the four loose enemy fields replaced by one `Swarm swarm` member. Builds,
plays full cycle.

**Concepts demonstrated:**
- SRP / C.4 / C.8: four fields that always change together + the functions that maintain them = one
  responsibility, one class. Same invariant-cluster reasoning as [[0008-scene-manager]] and the entity
  classes, now at aggregate level.
- Decoupling via return value: `Swarm::update` returns `bool breached` instead of writing
  `player.health = 0`; the scene owns the consequence. Swarm knows nothing about Player. (Callback to
  the condition-up/decide-at-caller pattern from L8.)
- Collisions kept as FREE functions (C.5) because they straddle two types — SRP is not "everything a
  method." User accepted this judgment cleanly.
- Scope discipline: extracted ONE responsibility, left ProjectilePool / HUD / collision-system as
  explicit future arcs. (Direct application of the L8 over-reach lesson — and this lesson ran smoothly
  in one pass, no hurdle.)

**Notable: user improved on the proposed design.** The lesson hedged on the game.h<->swarm.h include
cycle. User resolved it more cleanly than suggested: moved `Enemy` AND `MAX_ENEMIES` into `swarm.h`
(making it self-contained, no dependency on game.h), and `game.h` simply `#include "swarm.h"`. No cycle,
no incomplete-type issue. Good independent header-hygiene instinct — they're now reasoning about
dependency direction unprompted.

**Status:** scene_gameplay SRP arc started; swarm done. Remaining responsibilities still in the file:
object-pool init (bullets), cross-entity collision (correctly free), scoring/HUD draw. Candidate next
arcs: ProjectilePool class, HUD extraction (helper or overlay scene), per-scene state (pause). All
confirm-with-user.
