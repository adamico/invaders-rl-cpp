# First templates: overlaps<A,B> collision helper

User completed lesson 0013. Added header-only `include/collision.h` with
`template <typename A, typename B> bool overlaps(const A&, const B&)` wrapping CheckCollisionCircles on
`.pos`/`.radius`. Both collision functions now call `overlaps(bullet, enemy)` / `overlaps(state.player,
enemy)`. Builds, plays, behaviour unchanged.

**Concept (NEW mechanic — the templates gap from the learner profile is now opened):**
- Function template as a "recipe"; compiler instantiates one concrete fn per type pair.
- Compile-time duck typing: any type with `.pos`+`.radius` fits, no shared base class. Explicitly
  contrasted with the L6 `Scene` virtual hierarchy (runtime dispatch + vtable) vs templates
  (compile-time, zero runtime cost). User has now seen both polymorphism styles side by side.
- The key mechanical rule taught: template definitions must live in the header (instantiation needs the
  full body at each call site; a .cpp would give a linker undefined-reference). User's collision.h is
  correctly header-only, no collision.cpp.

**Honest scoping carried forward:** told the user the detect/effect split is overengineering for an
immediate-mode collision sweep (skip, or just rename Check* -> resolve*). The duplicated
`if (!active) continue;` across Swarm/ProjectilePool is the genuine remaining DRY target -> class
template Pool<T>, which the user has chosen as the next arc.

**Status:** function templates landed cleanly in one pass (consistent with the calibration that this user
absorbs new mechanics fast). Next: class template `Pool<T,N>` — bigger blast radius (touches every
`state.swarm.enemies` / `state.projectilePool.projectiles` / activeCount / deactivate site), so apply the
L8 lesson: enumerate ALL touched files with before/after, scope as composition, keep domain behaviour
(swarm movement, fire) in the owners. Note Pool can also re-home the activeCount invariant generically
(reinforces [[0013-tell-dont-ask]]).
