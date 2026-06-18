# Enemies shoot back — Phase 2 (finishing the game) begins

User completed lesson 0018, the first gameplay-completion feature after the 17-lesson C++ refactor arc.
Added a second `ProjectilePool` instance (`enemyProjectilePool`) to GameState, a `float enemyFireCooldown`
timer, a free function `updateEnemyFire` (accumulator + reservoir pick of one active enemy via the Swarm's
L16 begin/end), and `detectEnemyBulletPlayerCollisions` that decrements `player.health` per hit (health
finally a real resource, not a 0/1 flag). Wired update + draw for the new pool. Builds, plays well.

**Concepts:**
- Reuse of a generic type by INSTANTIATING IT AGAIN — the payoff of making ProjectilePool generic in L10/L14.
  New gameplay = composition of existing parts, not new plumbing.
- Accumulator/cooldown timer pattern (subtract dt, act + refill at zero) vs frame-counting.
- Reservoir sampling: uniform one-pass random pick over an unknown-length iterable
  (`if (GetRandomValue(1, ++seen) == 1) shooter = &e;`). raylib `GetRandomValue` (inclusive).
- Separate populations (player vs enemy bullets) as separate pool instances so the bullet-vs-enemy
  collision loop doesn't treat enemy fire as enemy-killing.

**Hiccup (user-caught, instructive):** wired `updateEnemyFire` to fire into `state.projectilePool` (the
player's pool) instead of `enemyProjectilePool`. Result: enemy shots entered the bullet-vs-enemy loop and
killed their own owners. User spotted the symptom ("enemy bullets killing their owners") and reasoned it was
a logic flaw before I pointed at the exact wrong-pool line. One-char-of-intent fix. Reinforces: separate
pools were correct; the bug was a misrouted call, not the design.

**Design smell user surfaced mid-apply (-> motivated L19):** to make enemy bullets differ (speed, then a
flipped sprite) they had to keep adding args to `fire()` and `spawn()` — telescoping/data-clump. User named
it themselves and asked to fix it properly. That became lesson 0019 (Introduce Parameter Object:
`ProjectileSpec` owned by the pool). Strong design instinct — spotting interface growth as a smell.

**Status:** Phase 2 underway. Game now has a two-sided fight and meaningful health. Next: L19 collapses the
telescoping back into a pool-owned spec. Remaining finishing features: real lives/respawn, bunkers, waves,
swarm speed-up. RL is NOT a direction — "RL" in the repo name = raylib (see [[MISSION.md]] phase 2).
