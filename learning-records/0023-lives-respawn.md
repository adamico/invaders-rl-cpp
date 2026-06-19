# Lives & respawn (classic Space Invaders model)

User completed lesson 0021 — the first Phase-2 feature landing in the new player.h/player.cpp TU.

**Design pivot the user drove (twice):** the lesson initially used the L20 modern model (hp per life × 3
lives = 15 hits). User flagged the hp<->lives redundancy ("are we going with a life = multiple hps?") and
chose the CLASSIC model: drop hp/PLAYER_HEALTH/health()/isAlive() entirely; `lives` is the only resource;
one hit = one life. Cleaner and authentic. Lesson rewritten to match.

**Final model:**
- `takeDamage()` (arg dropped) — any physical hit (enemy bullet OR body-contact with an enemy) -> loseLife()
  -> respawn if lives remain. User's second design call: body-contact should cost ONE life like any hit, not
  instakill (originally grouped under die()). So die() now means breach ONLY.
- `die()` — swarm breach only: `lives = 0`, instant unsurvivable game over.
- `respawn()` (private) self-places + sets invulnerableTimer; `loseLife()` (private) decrements + respawns.
- invulnerableTimer mercy window (accumulator) + blink draw via fmodf (<cmath>).

**Layering correction the user caught:** lesson first named the query `isGameOver()`. User: "should the player
know about the game?" — right, a layering violation. Renamed to `Player::isDead()` (player reports its own
state); the SCENE interprets a dead player as game over. Same model-vs-view line drawn earlier for feedback/
sound. "Game over" is scene vocabulary, not the entity's.

**Two linked bugs (user caught symptoms, I diagnosed):**
1. `loseLife()` condition inverted: `if (lives <= 0) respawn();` — respawned only when DEAD, never when lives
   remained. Fix: `if (lives > 0) respawn();`.
2. First spawn was invulnerable (reset()->respawn() sets the timer). Fix: `reset()` zeroes invulnerableTimer
   after the initial respawn — mercy follows a death, not the game start.
   These compounded: while first-spawn-invulnerable, takeDamage() early-returned, so "no respawn on damage"
   too. Both fixes together = correct behaviour.

**Concepts:** two-distinct-events-two-methods (takeDamage vs die, intent-revealing); public/private interface
(respawn/loseLife internal); timer-based invulnerability + fmodf blink (<cmath>); entity-reports-state /
scene-interprets-meaning (isDead not isGameOver). Mostly gameplay + reuse; the genuinely-new bits are the
layering discipline and the design judgment (which the user supplied).

**Open (deferred):** explicit `enum class PlayerState{Alive,Invulnerable,Dead}` vs current timer/lives — flagged
as possibly clarifying if state grows. Minor leftover: `takeDamage(int amount = 1)` still has the unused param
in player.h (classic model dropped it) — harmless, drop when tidying.

**Status:** Phase 2 progressing. Player fully owns its life-cycle + lives, scene just interprets isDead().
Next finishing feature (user's pick): waves (clear swarm -> faster/lower) or bunkers. See [[NOTES.md]].
