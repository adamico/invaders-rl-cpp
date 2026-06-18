# Lambdas via Pool::eachActive — passing the loop body as a callable

User completed lesson 0015. Added `template <typename F> void eachActive(F fn)` to `Pool` (mutating +
const overloads) and converted the four do-to-all loops — swarm horizontal-move, swarm draw, projectile
update, projectile draw — to `pool.eachActive([&](...){ ... })`. Builds, runs, behaviour identical.

**Concepts (NEW mechanic: lambdas):**
- Lambda anatomy: capture list / params / body; `[&]` capture-by-reference for live per-frame state
  (direction, speed, deltaTime, texture). Contrast with `[=]` (copy) and `[]` (none).
- Passing a callable to a templated parameter `F` -> inlined, zero overhead vs `std::function` boxing.
  Ties back to the template lessons (L13/L14).
- const + non-const overloads so `draw()` (const method) binds the const `eachActive` taking
  `const T&` — const-correctness (L4) flowing through a template.
- **The boundary that was the point:** the two break-early loops in `Swarm::update` (edge-detect, breach)
  were correctly LEFT as raw `for` loops — a lambda can't break the caller's loop; eachActive is for
  "do to all," not "find one and stop." User respected this and did not force them through eachActive.
  `reset()` also correctly left raw (it touches inactive slots).

**Style note (carries the L0015 side-thread on naming conventions):** user named the method `eachActive`,
not the lesson's `each_active` — deliberately applying the camelCase house style we'd just discussed
(STL uses snake_case; match-the-codebase wins). Consistent self-correction; this is now a settled habit.

**Status:** lambdas landed in one pass. The Pool abstraction (storage + acquire + activeCount +
eachActive) is now a tidy, reusable generic container; the `if (!active) continue;` boilerplate is gone
from every do-to-all site. The C++ mechanics arc (templates -> class templates -> lambdas) is well
covered.

**Open / next candidate arcs (confirm with user):**
- Make `Pool::items` private now that acquire/activeCount/eachActive cover most access. What still reaches
  in: `Swarm::reset` (writes items[i] to build formation) and the collision free-functions in
  scene_gameplay (loop over .pool.items). Would need an accessor or to route those through the API.
- A find/any-shaped helper for the break-early loops (edge-detect/breach), if worth it.
- Collisions via nested eachActive (judgment call — nesting lambdas may hurt readability; flag, don't push).
- C++20 concepts to name the implicit element requirements (.active, .pos/.radius).
- Per-scene state / pause overlay (scene stack) — the remaining gameplay-feature arc, still untouched.
