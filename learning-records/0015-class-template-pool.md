# Class template Pool<T,N> — composed into Swarm + ProjectilePool

User completed lesson 0014. Added header-only `include/pool.h` with `template <typename T,
std::size_t N> struct Pool` exposing `items`, `acquire()` (first inactive slot), and a derived
`activeCount()`. Both `Swarm` and `ProjectilePool` now COMPOSE a `Pool` (has-a, not is-a) and keep
their bespoke behaviour (swarm movement/breach, projectile fire). Builds, plays, behaviour unchanged.

**Concepts (NEW mechanic):**
- Class templates with two parameter kinds: `typename T` + non-type `std::size_t N` (compile-time
  array size). `Pool<Enemy,55>` / `Pool<Projectile,100>` are distinct generated classes.
- Composition over inheritance for sharing a mechanism (C.129): the owners use the pool's storage but
  aren't a kind of pool.
- **Sequel-to-L12 payoff landed:** Swarm deleted its stored `activeCount` field; the count is now
  DERIVED (`pool.activeCount()` counts active flags on demand). No stored value -> no invariant ->
  can't drift. Framed as "the safest state is state you don't duplicate" — the deeper move past L12's
  "maintain it safely." User got it and applied cleanly.
- `Swarm::deactivate` collapsed to `enemy.active = false` but the interface was kept, so Tell-Don't-Ask
  call sites were untouched. `fire()` collapsed onto `acquire()` + existing `Projectile::spawn`.
- Header-only template rule reinforced (no pool.cpp). Encapsulation kept blast radius small: only 3
  reach-in sites in scene_gameplay (.pool.items x3, activeCount() x1) — public method surface unchanged.

**Minor:** user renamed the pool method `active_count()` -> `activeCount()` for camelCase consistency
with the rest of the codebase. Good instinct (matches house style).

**Status:** two template lessons done (function L13 + class L14); the templates gap from the learner
profile is now substantively covered. Applied in one pass, working — calibration holds: user absorbs new
mechanics fast and self-corrects style.

**Open / next candidate arcs (confirm with user):**
- `Pool::each_active(fn)` taking a callable -> introduces LAMBDAS (genuinely new), kills the last
  `if (!active) continue;` duplication AND the two-dot `state.swarm.pool.items` Demeter reach.
- Make `Pool::items` private once each_active exists.
- C++20 concepts to name the implicit `.active` / Collidable requirement (turns cryptic instantiation
  errors into clear messages).
- Per-scene state / pause overlay (scene stack) — still untouched, the remaining gameplay-feature arc.
