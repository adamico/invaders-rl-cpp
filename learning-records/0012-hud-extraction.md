# HUD extraction: the "when NOT to make a class" lesson

User completed lesson 0011 quickly ("fast and easy"). Extracted the HUD draw out of
`SceneGameplay::draw` into a free function `drawHud(int health, int score)` in new `include/hud.h` +
`src/hud.cpp`. Builds, plays, HUD renders unchanged.

**Concept demonstrated — the contrast point:** after two class extractions (Swarm, ProjectilePool),
this one deliberately is NOT a class. The HUD owns no state and protects no invariant, so it's a free
function. Reinforces C.4 from the opposite direction: the same rule that kept collisions free also says
a stateless renderer shouldn't be a class. SRP = "one job per unit," not "everything is an object."

**Decoupling applied correctly:** `drawHud` takes primitives (`int health, int score`), NOT
`const GameState&`. `hud.h` includes nothing game-specific — zero dependency on the rest of the game.
User got this right with no correction. Also kept the frame-lifecycle boundary clean: `drawHud` does not
call Begin/EndDrawing — the scene still owns those.

**Speed signal:** user reported it as easy and applied it cleanly in one pass (params reordered to
health,score but consistent throughout). Suggests SRP + class-vs-free-function judgment is now solid;
the lesson landed mostly as confirmation rather than new struggle. Calibration note: this user is now
comfortable enough with these extractions that pure-repetition lessons add little — next lessons should
introduce genuinely new mechanics (scene stack / per-scene state, templates) rather than more of the
same pattern.

**Status:** `scene_gameplay.cpp` now down to: Projectile::spawn + its constants, the two free collision
functions, InitPlayer/InitGameplay, and the scene update/draw orchestration. The big visible SRP work is
done. Next candidate arcs: per-scene state / pause overlay (scene stack — genuinely new: scenes holding
own data + composing scenes), a shared text helper, or a templated Pool<T> over Swarm+ProjectilePool
(possibly premature). Confirm with user.
