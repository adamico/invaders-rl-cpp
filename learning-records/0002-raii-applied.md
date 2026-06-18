# RAII conversion of GameResources completed and understood

User converted `GameResources` from a plain struct + free `Load/UnloadGameResources` into a class
with constructor (acquire) / destructor (release) and `= delete`d copy ctor + copy assignment.
Builds and replays cleanly (gameover → title → gameplay).

**Evidence of real understanding, not just coverage:**
- Diagnosed that the deleted copy ctor was caught by the `*state = (GameState){}` save-zero-restore
  trick in `InitGameplay`, and replaced it with an explicit field reset rather than fighting the ban.
- Independently spotted/handled the replay bugs I flagged: reset `activeEnemies = 0` before
  `InitEnemies`, and wrote a new `InitBullets` (loop setting `active = false`) mirroring the existing
  `InitPlayer`/`InitEnemies` structure — good instinct for consistency.
- Correctly reasoned that `currentScene` need not be reset in `InitGameplay` because the
  title→gameplay transition owns it.

**Implications for next sessions:** User grasps RAII, copy semantics, the rule-of-three motivation,
designated initializers, and is comfortable refactoring against compiler errors. Did NOT need move
semantics yet (no ownership transfer in play) — defer move/`std::unique_ptr` until a scene or entity
needs to hand a resource off. Next: `constexpr` to replace the `#define` constants wall
(low risk, sets up typed constants before entity classes). See [[NOTES.md]] roadmap.
