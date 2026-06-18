# SRP rep 2: extracted ProjectilePool (object pool) — applied from memory

User completed lesson 0010. Created `include/projectile_pool.h` + `src/projectile_pool.cpp` with a
`ProjectilePool` owning `std::array<Projectile> projectiles` and `reset()` / `fire(from, dir)` /
`update(dt)` / `draw(texture)`. `GameState.bullets[]` replaced by a `ProjectilePool projectilePool`
member. Builds, plays.

**Key signal — applied "from code only" (from memory, without re-reading the lesson).** This was the
intended retrieval-practice payoff of L10 being a deliberate second rep of the L9 swarm extraction. The
SRP / extract-a-pool pattern is now stored, not just fluent.

**Concepts demonstrated:** object-pool pattern named explicitly; mirror of [[0010-gameplay-srp-swarm]]
... (Swarm). `fire` returns bool so the caller owns the laser sound — same report-a-fact/caller-decides
decoupling as `Swarm::update`. User independently added a `dir` param to `fire` (the "enemies can shoot
too" hook from the lesson's questions) — forward-looking design instinct.

**Half-landed on first pass, then corrected (good teaching moment):** initial `PlayerShoot` still (a)
looped over `projectilePool.projectiles` to find a slot — duplicating the search `fire()` already does,
leaking pool internals back into the caller — and (b) called `PlaySound` unconditionally before the
loop, so the latent "sound on full pool" bug wasn't fixed. Pointed both out; user collapsed it to the
intended `if (pool.fire(...)) PlaySound(...)`. Lesson reinforced: extracting a method only pays off if
the caller stops re-implementing what the method now owns.

**Status:** entity-cluster SRP done — both Swarm and ProjectilePool extracted; collisions correctly
remain free functions. `scene_gameplay.cpp` now holds: Projectile::spawn + constants, the two free
collision functions, scoring, HUD draw, and scene orchestration. Next candidate arcs: HUD extraction
(helper or overlay scene), per-scene state (pause), or a templated shared pool abstraction (likely
premature — flagged for discussion, not yet warranted).
