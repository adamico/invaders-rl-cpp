# Behaviour on objects / Tell-Don't-Ask round 2 (from the user's own REFACTOR notes)

User completed lesson 0020, sourced from five `// REFACTOR` breadcrumbs they had left in scene_gameplay.cpp.
Theme: behaviour belongs with the data WHEN there's an invariant/rule to protect (the sharpened version of
Tell-Don't-Ask + the L11 "when NOT to make a class" judgment). Applied in three committed checkpoints.

**Part 1 (commit 66b2bd9) — Player owns its health:** `hp` made private behind `takeDamage(int amount = 1)`
(clamps at 0), `die()`, `isAlive()`, and `health()` (read-only query for the HUD). Collision sites now TELL:
`state.player.die()` / `state.player.takeDamage()` instead of poking `health = 0` / `health -= 1`.
Privatising a data member made Player a non-aggregate, which broke the `(Player){.pos=…}` designated init ->
replaced with `Player::reset(Vector2)` (same shape as Swarm/Pool reset; taught as a consequence, not a wart).

**Part 3 (commit 56b9a3e) — Command-Query Separation:** the opaque `bool Swarm::update()` ("unclear return"
per user's own note) split into `void update()` (command, sets a private `bool breached` member) +
`bool hasBreached() const` (query). Scene reads `if (state.swarm.hasBreached()) state.player.die();`.
User asked a sharp question here: does making `breached` private require a `breach()` setter? Answer: no —
it's written only inside `update()`, a member fn, so it sets the private field directly; privacy blocks only
EXTERNAL access. Good mental-model check.

**Part 4 (commit 088143e) — selection belongs on the owner + std::optional:** user had spotted (in L18)
that `updateEnemyFire` did two jobs. Moved the reservoir pick onto `Swarm::randomShooterPosition() const`
returning `std::optional<Vector2>` — returns the position or `std::nullopt`, never leaks a mutable `Enemy*`
(keeps the L16 encapsulation). `updateEnemyFire` shrinks to timer + `if (auto pos = …) fire(*pos)`. NEW idiom:
std::optional (`<optional>`, nullopt, `if (auto x = ...)` truthiness, `*x` deref).

**Bug caught in review (mine):** user first placed the `if (!shooter) return std::nullopt;` guard INSIDE the
reservoir loop. Two faults: dead code (first active enemy always sets shooter), and an all-inactive swarm
falls through to `return shooter->pos;` = null deref / crash (dodged today only because swarm-cleared triggers
a same-frame victory scene transition). Fixed: guard moved AFTER the loop. Good reminder that "it builds and
seems to run" hides latent crashes on edge states.

**Part 5 — the deliberate no-op:** `score` stays a bare `int` (`state.score += enemy.scoreValue;`). User
chose this when asked. Rationale logged in the lesson: score has no invariant (only ever +=, no clamp/derived
state), so a `Score::add()` wrapper would be a trivial setter (C.131/YAGNI). The CONTRAST with health (which
does have rules) is the actual teaching point. Revisit if score grows a multiplier or persisted high-score.

**Open question user raised (deferred, not built):** should `die()`/`takeDamage()` trigger feedback (sound,
screen-shake)? Answered: NO — entity owns simulation, scene owns feedback (model vs view); coupling Player to
audio/camera/resources would wreck testability + headless stepping. Refinement for later: have `takeDamage()`
RETURN whether damage applied so the scene can react, keeping presentation out of the domain object. This is
the seed of a future "juice" (screen-shake/hit-flash/sound) presentation-layer arc.

**Status:** Phase 2 progressing well. Player/Swarm now fully behaviour-encapsulated; the scene reads as intent.
Next finishing feature (user's pick pending): real lives + respawn (short now that takeDamage/isAlive exist) or
bunkers. See [[NOTES.md]].
