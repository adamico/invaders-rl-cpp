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
    error (the expected proof). CODA (+2): Demeter forwarding (begin/end/activeCount on Swarm+
   ProjectilePool) -> dropped all caller `.pool` -> made pool/direction/speed PRIVATE (struct->
   class Swarm). Encapsulation now composes one level up. See LR-0017.
LESSON-WRITING FEEDBACK: in "final shape" summary blocks, DON'T abbreviate template method bodies to a
bare `;` - reads as "move to .cpp" which is impossible for templates. Show real bodies / mark elision.
17. [DONE L0017] C++20 concepts. Collidable (overlaps: pos+radius) + Poolable (Pool's T: active).
    bare typename -> named/checked constraints; errors at boundary not body; zero codegen change.
    Real session content = TOOLCHAIN: c++17->c++20 in 3 places (Makefile + .clangd + compile_commands;
    clangd reads .clangd not Makefile -> "concept unrecognized" was stale -std). Stale 3-entry DB (ref'd
    deleted player.cpp) regenerated via `bear -- make` -> 8 TUs. IWYU: <concepts> was transitive via
    <array>, added explicit. User spotting tooling smells unprompted. See LR-0018.
=== C++ IDIOMATIC-REFACTOR ARC COMPLETE (17 lessons). ===

=== PHASE 2: FINISH THE GAME (raylib gameplay, refactor-driven) ===
NOTE: "RL" in repo name = RAYLIB, not reinforcement learning. User confirmed; NOT doing ML/agent/env.
MISSION.md phase-2 section + memory updated. Candidate features: enemy fire, bunkers, lives/respawn,
waves, swarm speed-up, juice.
18. [DONE L0018] Enemies shoot back. 2nd ProjectilePool instance (generic-type reuse payoff) + cooldown
    timer + reservoir-pick shooter (Swarm begin/end) + enemy-bullet->player health damage (health now a
    real resource). Hiccup: fired into player pool by mistake -> enemies self-killed (user caught). Mid-
    apply user surfaced telescoping-args smell on fire/spawn -> motivated L19. See LR-0019.
19. [DONE L0019] Introduce Parameter Object: ProjectileSpec{speed,dir,flipVertical} OWNED by the pool
    (set once in reset(spec)); fire() collapses to position-only. C++20 designated inits. Fixed directional
    retire bug (pos.y>0 leaked downward bullets -> both-edge check) + vertical flip (negative source-rect
    height). User made specs constexpr named constants (nice). Discussed Type Object pattern kinship
    (this = simplified value-copy form; full form = shared Breed* + data table, YAGNI for 2 kinds). LR-0020.
20. [DONE L0020] Behaviour-on-objects / Tell-Don't-Ask r2, from user's own // REFACTOR breadcrumbs. 3
    commits: (1) Player hp private behind takeDamage(int=1)/die/isAlive/health() -> forced Player::reset
    (aggregate->class consequence); (2) swarm.update bool -> void update + hasBreached() query (CQS);
    (3) Swarm::randomShooterPosition()->std::optional<Vector2> (user's SRP catch; new idiom std::optional).
    Score DELIBERATELY stays int (C.131/YAGNI - contrast IS the lesson). Review caught a null-deref:
    nullopt guard was inside the reservoir loop -> moved after. Deferred: feedback(sound/shake) stays in
    scene not entity (model vs view) -> seeds a future "juice" arc. See LR-0021.

21b. [DONE, inline refactor not numbered lesson] Promote Player to own TU: player.h (self-contained, owns
    PLAYER_* constants) + player.cpp (all Player::* defs moved out of scene_gameplay.cpp) + NEW canvas.h
    (shared CANVAS_* render constants, avoids coupling Player to kitchen-sink game.h). Header = dependency
    surface; constexpr@namespace = internal linkage (safe in headers); owner-owns-its-constants. User took
    the clean option: Player::reset() now no-arg + self-places (scene's PLAYER_RADIUS leak gone). Basis:
    CG SF.11/SF.2/SF.10 + Lakos. See LR-0022.

21. [DONE L0021] Lives & respawn, CLASSIC model (user chose: dropped hp entirely, lives = only resource,
    one hit = one life). takeDamage()=any physical hit (bullet+body contact)->loseLife->respawn; die()=
    breach only (instant gameover). invuln mercy window + fmodf blink. User caught layering: isGameOver()
    -> isDead() (player reports own state, scene interprets). 2 linked bugs fixed: inverted loseLife
    condition + first-spawn invuln. Player now fully owns life-cycle. See LR-0023.

NEXT (Phase 2, user's pick pending): waves (clear swarm -> faster/lower next wave) OR bunkers (destructible
cover). Deferred: enum class PlayerState if state grows; drop unused takeDamage(int) param when tidying;
"juice" presentation arc (screen-shake/flash/sound at collision sites).
Future presentation arc: "juice" (screen-shake/hit-flash/sound) at the collision sites; takeDamage() could
return whether damage applied so scene drives feedback. Type Object full form if weapon/enemy variety grows.
Possible future arcs (confirm with user before starting a new mission):
- C++20 concepts (name element requirements); Swarm forwarding own iteration (Demeter); REVISIT RL
  GOAL (mission change per MISSION.md — confirm + new learning record) as C++ refactor arc completes.
- Per-scene state as class members (pause menu, settings, level-complete).
- Revisit the repo's original RL-environment goal (currently out of scope in MISSION.md).

## Deferred tidy-ups
- (resolved) Mixed screen-size source — unified on windowSize in L0005.
