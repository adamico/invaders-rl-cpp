# FOR_EACH macros replaced with range-for; reference-vs-copy understood

User converted all 9 `FOR_EACH_PROJECTILE`/`FOR_EACH_ENEMY` macro loops in `scene_gameplay.cpp`
to range-based for, correctly using `Projectile&`/`Enemy&` (reference) on every one — including the
nested loop in `CheckBulletEnemyCollisions` — and deleted the macro definitions. Builds and plays;
no frozen-entity bug, so the copy-vs-reference trap was avoided.

Correctly left the two index-based loops (`InitEnemies`, `InitBullets`) as classic `for` because
they need the index for grid column/row math — good judgment about when range-for does/doesn't fit.

**Implications:** User reliably picks reference vs copy. Did NOT yet apply `const&` to the read-only
draw/scan loops — flagged as a lesson-4 preview (const-correctness becomes load-bearing once these
loops live inside class methods). `std::array` conversion offered but treat as optional/not-yet-done
unless the user confirms. Next: lesson 4 — turn Player/Enemy/Projectile into classes that own their
own update/draw, the first big structural shift toward the mission. See [[NOTES.md]].
