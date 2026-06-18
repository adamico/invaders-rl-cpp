# Teaching Notes

## Learner profile
- Knows C well; "some C++" but shaky on modern idioms (RAII, smart pointers, move, templates).
- Goal is better game structure, not C++ theory for its own sake.

## Preferences
- **Refactor-driven**: each lesson must touch real code in this repo. No toy examples.
- **Keep it buildable**: game compiles & runs after every lesson.
- Caveman/terse comms in chat (global pref). Lessons themselves stay full and beautiful.

## Starting code state (2026-06-18)
- C-in-cpp: `GameState*` threaded through free functions; manual Load/Unload; macros for
  loops & constants. Files: main.cpp, scene_gameplay.cpp, scene_title.cpp, scene_gameover.cpp.

## Refactor roadmap (zone-of-proximal order)
1. [DONE L0001] RAII resources
2. [DONE L0002] constexpr instead of #define constants
3. [DONE L0003] range-for / std::array instead of FOR_EACH_* macros
4. [DONE L0004] const-correctness (draw path) — bridge step before methods
5. [DONE L0005] Entity classes — Projectile/Player/Enemy own update()/draw()/spawn; swarm logic
   stays free (C.4). Screen-size unified on windowSize constexpr.
6. [DONE L0006] Scene base class + polymorphism — virtual update/draw, virtual dtor, unique_ptr +
   move, makeScene factory. Bonus: GameScene -> enum class (scoped, strong-typed).

## Core C->C++ port DONE (L1-L6). Now in extension arc.
7. [LESSON DELIVERED L0007, awaiting apply] SceneManager class owns unique_ptr + transition
   bookkeeping; main shrinks to `scenes.tick(state, dt)`. Teaches invariant-pair-as-class,
   ctor establishes invariant (C.40/41), explicit single-arg ctor (C.46), link vs compile error.
8. [DONE L0008] update() returns unique_ptr<Scene> -> killed makeScene switch + GameScene enum +
   state.currentScene + shown_. Hurdle (see LR-0009): derived-header return types, deep-void-helper
   condition pattern, double-update bug. Lesson patched to be self-contained.
9. [DONE L0009] SRP: extracted enemy Swarm class (reset/update/draw, swarm.h+.cpp), GameState shrank
   4 fields -> Swarm member. Swarm::update returns bool breached (scene decides death). Collisions
   stay FREE (C.4/C.5). User self-resolved include cycle better than lesson (moved Enemy+MAX_ENEMIES
   into swarm.h, self-contained). One-pass, no hurdle. See LR-0010.
10. [DONE L0010] ProjectilePool: mirror of Swarm (reset/fire/update/draw). fire() returns bool ->
    caller gates laser sound (fixed latent full-pool-sound bug). GameState bullets[] -> projectilePool.
    Applied FROM MEMORY (retrieval practice worked). User added dir param to fire (enemies-shoot hook).
    First pass leaked pool internals in PlayerShoot (kept slot loop + unconditional sound); corrected.
    See LR-0011.
11. [DONE L0011] HUD extraction — "when NOT to make a class." Stateless -> FREE function
    drawHud(health, score), primitives not GameState&, hud.h includes nothing game-specific. Applied
    clean in one pass, user called it easy. CALIBRATION: SRP/class-vs-free judgment now solid; pure-rep
    lessons add little -> next lessons need genuinely NEW mechanics (scene stack, templates). LR-0012.
12. [DONE L0012] Tell-Don't-Ask / invariant ownership. USER spotted the smell themselves. Fix:
    Swarm::deactivate(Enemy&) idempotent guard owns flag+count atomically; Projectile::deactivate()
    inline. Collision fns stay FREE but TELL not poke. Verified: 0 activeCount writes outside swarm.cpp.
    See LR-0013. Side: header-inline methods (implicitly inline/ODR-safe), clang-format short-if.
13. [LESSON DELIVERED L0013, awaiting apply] Function templates (NEW MECHANIC — biggest profile gap).
    Motivated by duplicated CheckCollisionCircles in the two collision fns. overlaps<A,B>(a,b) in
    header-only collision.h = compile-time duck typing (any type w/ .pos+.radius), contrasted vs L6
    runtime virtual dispatch. Key gotcha taught: template defs MUST live in headers (else linker
    undefined-ref). Addressed deferred smells honestly: detect/effect split = overengineering for
    immediate-mode (skip, maybe just rename); if(!active)continue dup -> next arc = class template.
14. [DONE L0014] Class template Pool<T,N> (typename + non-type param), composed into Swarm+
    ProjectilePool. Derived activeCount() -> stored field deleted (sequel-to-L12 payoff). fire() ->
    acquire()+spawn. Header-only. Applied one pass, working. User renamed method to camelCase
    activeCount(). See LR-0015. Templates gap now substantively covered (L13 fn + L14 class).
15. [DONE L0015] Lambdas via Pool::eachActive(F) (named eachActive, camelCase house style). Mutating+
    const overloads, 4 do-to-all loops converted; break-early loops (edge/breach) + reset correctly
    left raw. [&] capture. One pass, working. See LR-0016. Mechanics arc (fn templates -> class
    templates -> lambdas) well covered.
16. [DONE L0016] Encapsulation capstone — Pool::items now PRIVATE behind container interface
    (begin/end +const, operator[] +const, struct->class). 5 sites converted. NEW mechanics: range-for
    protocol, operator overloading, struct-vs-class. Hiccups: (a) misread collapsed-body `;` shorthand
    as "split to .cpp" - clarified templates are header-only; (b) missed scene_gameplay:122 -> private
    error (the expected proof). See LR-0017.
LESSON-WRITING FEEDBACK: in "final shape" summary blocks, DON'T abbreviate template method bodies to a
bare `;` - reads as "move to .cpp" which is impossible for templates. Show real bodies / mark elision.
=== C++ IDIOMATIC-REFACTOR ARC COMPLETE (16 lessons). ===
Possible future arcs (confirm with user before starting a new mission):
- C++20 concepts (name element requirements); Swarm forwarding own iteration (Demeter); REVISIT RL
  GOAL (mission change per MISSION.md — confirm + new learning record) as C++ refactor arc completes.
- Per-scene state as class members (pause menu, settings, level-complete).
- Revisit the repo's original RL-environment goal (currently out of scope in MISSION.md).

## Deferred tidy-ups
- (resolved) Mixed screen-size source — unified on windowSize in L0005.
