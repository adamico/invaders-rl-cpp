# update() returns next scene — last switch + GameScene enum killed

User completed lesson 0008. `Scene::update` now returns `std::unique_ptr<Scene>` (nullptr = stay,
non-null = replace). Deleted: `makeScene` + its switch (and `src/scene.cpp`, since the build globs
`src/*.cpp`), the `GameScene` enum, `state.currentScene`, and `SceneManager::shown_`. `SceneManager`
ctor now takes `unique_ptr<Scene>`; `main` constructs `SceneManager scenes(make_unique<SceneTitle>())`.
Each scene's `.cpp` now includes its successor's header for `make_unique`. Builds; full cycle (play →
die/win → gameover → enter → restart) works.

**Concepts demonstrated:** return-by-value move out of a function (F.20); nullptr-as-sentinel vs
optional; distributed-successors-vs-central-factory design axis (cohesion vs coupling); include-in-.cpp
to break cycles; restart side-effect (`InitGameplay`) preserved alongside the return.

**Where it was a hurdle (process learning — lesson was under-specified, patched after):**
1. Changed `Scene::update` return type in base + .cpp but NOT the derived **headers** → "different return
   type than override" errors. Headers declare it too.
2. `scene_manager.h`/`.cpp` end-state and the `rm src/scene.cpp` step weren't shown — only mentioned in
   prose. Added explicit code blocks.
3. **Deep-helper transition**: the gameover trigger lived inside `void` helpers (`UpdateEnemies`,
   `CheckPlayerEnemyCollisions`) that can't return a scene. Resolution taught live: helpers mutate
   *condition state* (`player.health = 0` / `victory`), and `update` reads conditions at a single
   decision point and returns the scene. Also hit UB from a helper returning `unique_ptr<Scene>` that
   fell off the end on the false path.
4. **Double-update bug**: leftover `current_->update()` after the `if (auto next = current_->update())`
   line → sim stepped twice/frame → player moved at 2x speed. Diagnosed from the symptom by the user
   ("double update somewhere?"), confirmed in tick. The `if` line IS the update call.

All four are now folded into lesson 0008. Status: extension arc complete through L8; the scene system is
fully polymorphic with no enum/switch. Possible next: per-scene state (pause/settings as members),
fixed-timestep update/render split (ties to dormant RL determinism goal — mission change, confirm first).
