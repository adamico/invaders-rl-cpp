# SceneManager: encapsulating ownership + transition bookkeeping

User added a `SceneManager` class (lesson 0007) that owns the `unique_ptr<Scene> current_` and the
`shown_` mirror, with `explicit SceneManager(GameScene start)` + `tick(GameState&, float)`. `main`
shrank to constructing the manager and calling `scenes.tick(state, GetFrameTime())` in the loop body.
Builds; full Title→Gameplay→Gameover→Title cycle works, behaviour identical to L6.

**Concepts demonstrated:**
- **Invariant-pair-as-class**: `current_`/`shown_` must always agree; making them private members lets
  the class enforce sync. SRP — `main` runs the game, doesn't babysit transitions. (C.4 callback to L5.)
- **Constructor establishes the invariant** via member init list (C.40/41); `explicit` single-arg ctor
  (C.46). Same RAII-setup reasoning as `GameResources` (L1).
- Move semantics (L6) now an implementation detail hidden inside `tick`.
- Link vs compile error distinction reinforced (forgot/added `scene_manager.cpp` to build).

**Wisdom Q (asked unprompted, good sign):** could `tick` split into `update()`/`render()` for a
fixed-timestep loop? Answered yes — the split is *forced* by differing call counts (update 0..N times,
render once per frame); decouples sim rate from render rate for determinism. Tied back to the dormant
RL goal (determinism matters for training/replays). Advised deferring until a concrete reason appears
(stutter / RL revival) — premature now. Interpolation deferred further.

**Status:** extension arc past the core port. Last factory switch still in `makeScene`.
Next (teased L8): `update` returns the next scene → remove the factory switch + possibly `GameScene`
enum. Fixed-timestep split is a candidate arc once/if the RL goal is revived (would be a mission change
per [[MISSION.md]] — confirm first).
