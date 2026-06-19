# Introduce Parameter Object: ProjectileSpec owned by the pool

User completed lesson 0019, motivated by a smell THEY surfaced mid-L18: enemy-vs-player bullets differed by
speed (then a flipped sprite), and each difference was telescoping `fire()`/`spawn()` with another argument.
Fixed via Introduce Parameter Object (Fowler): a `ProjectileSpec { float speed; Vector2 dir; bool
flipVertical; }` with default member initializers. The pool OWNS one spec, set once in `reset(ProjectileSpec)`;
`fire(Vector2 from)` collapsed back to position-only; `spawn(Vector2, const ProjectileSpec&)` copies traits in.
Builds, plays: player bullets up@500, enemy bullets down@350 flipped.

**Concepts:**
- Introduce Parameter Object / data-clump + long-parameter-list smell. Key insight = OWNERSHIP: the traits are
  per-weapon (constant for the pool's life), not per-shot, so the pool holds them and fire() shrinks to what
  truly varies (position). Interface stops growing: a future 4th trait = one spec field, zero call-site edits.
- C++20 designated initializers + default member initializers (`{.speed=…, .dir=…, .flipVertical=true}`) —
  cashes in the c++20 enabled in L17.
- `const ProjectileSpec&` param = read-only, no copy.
- (raylib) vertical flip via NEGATIVE source-rect height in DrawTextureRec.

**Two bugs folded in & fixed:**
- Directional retire: `active = pos.y > 0` never retired DOWNWARD enemy bullets (stayed >0 past the bottom,
  leaking pool slots) -> `pos.y > 0 && pos.y < GetScreenHeight()` (retire off either edge).
- (the L18 self-kill wrong-pool bug was already fixed before this lesson.)

**User style wins:** made the two specs `constexpr` named constants (PLAYER_PROJECTILE_SPEC /
ENEMY_PROJECTILE_SPEC) rather than inline literals — cleaner than the lesson showed. Named the field
`flipVertical` (more explicit than lesson's `flipV`). Consistent house-style self-correction continues.

**Pattern connection (user-initiated):** user asked whether ProjectileSpec relates to Nystrom's Type Object
pattern. It does — it's the simplified value-copied form (pool owns spec, projectile copies fields at spawn)
vs canonical Type Object (instance holds a shared `Breed*`, kinds defined as data at runtime). Flagged: evolve
to full Type Object (`const ProjectileSpec*` on the projectile + a spec table) only if weapon/enemy variety
grows to many data-defined kinds. For two kinds, value-copy is the right amount (YAGNI).

**Status:** Phase 2 continues. Next is L20 (behaviour-on-objects / Tell-Don't-Ask round 2) built from the
user's own `// REFACTOR` breadcrumbs (player.die/takeDamage, opaque swarm.update bool, score stays int) plus
the SRP catch they made in updateEnemyFire (-> Swarm::randomShooterPos returning std::optional). See
[[NOTES.md]].
