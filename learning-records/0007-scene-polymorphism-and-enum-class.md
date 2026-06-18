# Capstone done: scene polymorphism (virtual + unique_ptr + move) and enum class

User replaced the `GameScene` enum + per-frame dispatch `switch` with an abstract `Scene` base
(pure virtual `update`/`draw`, virtual destructor) and `TitleScene`/`GameplayScene`/`GameoverScene`
overrides, owned via `std::unique_ptr<Scene>` in `main`, built by a `makeScene` factory (the single
remaining switch). Then converted `GameScene` to a scoped `enum class`. Builds; full transition cycle
Title→Gameplay→Gameover→Title works.

**Concepts demonstrated (debugged live):**
- `virtual` = runtime dispatch; `= 0` pure virtual / abstract base; `override` safety keyword.
- **Virtual destructor** necessity for deletion through a base pointer (taught proactively).
- **`std::unique_ptr` + move semantics** — the move deferred since lesson 1 finally landed:
  move-only ownership, factory returns by value, `current = makeScene(...)` move-assigns and deletes
  the old scene. Same non-copyable reasoning as `GameResources` (callback to L1).
- Reference vs pointer at call sites: hit `&state` (pointer) not binding to `GameState&`; fixed.
- **Include mechanics:** `<memory>` header purpose; include guard collision ("redefinition of Scene")
  from a missing/duplicated guard; needing full definition (not forward decl) to inherit; factory
  needing all derived headers. Good real-world header-hygiene experience.
- **`enum class`** rationale understood correctly: user asked if it would prevent the earlier typo;
  accepted the honest answer that the typo was already a compile error, and that enum class's real wins
  are scoping (no global pollution / name clashes) and strong typing (no implicit int conversion), not
  typo-catching. Migrated all 11 sites with `GameScene::` qualification; no static_cast needed.

**Status: the C→C++ port roadmap (NOTES.md) is COMPLETE.** User now independently reasons about
ownership, member-vs-free design (C.4), const propagation, virtual dispatch, and header hygiene.
Future directions if they continue: a `SceneManager` class owning the unique_ptr + transitions;
`update` returning the next scene to remove the last switch; per-scene state via class members;
revisit the repo's original RL-environment goal (currently out of scope per [[MISSION.md]]).
