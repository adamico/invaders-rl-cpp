# Encapsulation capstone: Pool::items private behind a container interface

User completed lesson 0016 (motivated by their own question: "can Pool::items finally go private?").
Rounded `Pool` into a real container: added `begin()`/`end()` (+ const overloads), `operator[]`
(+ const), switched `struct Pool` -> `class Pool` with explicit `public:`/`private:`, moved
`std::array<T,N> items` under `private:`. Converted all 5 holdout sites (swarm reset index-write ->
`operator[]`; swarm break-loops + both reset all-touch + collision loops -> range-for over the pool).
Builds, runs, behaviour identical.

**Concepts (NEW mechanics):**
- Range-for protocol: `for (x : c)` desugars to `c.begin()`/`c.end()`; providing them makes a type
  iterable (duck-typed, no base class). Range-for is a REAL loop in the caller, so it serves the
  break-early loops that `eachActive` (a lambda) could not.
- `operator[]` overloading (+ const overload) for indexed access; returns `T&` so `pool[i] = {...}`
  assigns into the slot.
- struct vs class = default access only (struct public, class private); convention C.2/C.8: struct for
  passive data (Enemy/Projectile stay struct), class once there's a deliberate interface over private
  state.
- Principle: expose operations, not representation -> storage can change later without touching callers.

**Teaching-moment hiccups (both quick, both instructive):**
1. User read the lesson's "final shape" code block (bodies collapsed to `T* acquire();` as shorthand for
   "body as before") as an instruction to split declaration from definition. Clarified: template bodies
   MUST stay in the header (header-only rule from L13/L14); pool.h was already correct. -> FEEDBACK: in
   lesson "final shape" summaries, don't abbreviate template method bodies to a bare `;` — it reads as
   "move to .cpp," which is impossible for templates. Show real bodies or clearly mark the elision.
2. Hit `'items' is a private member` on build — had missed converting scene_gameplay.cpp:122
   (state.swarm.pool.items). This was the EXPECTED proof-of-encapsulation error; user understood once
   pointed at it, fixed the last site. Reinforced that the compiler error IS the verification.

**Coda — Demeter forwarding + one level up (recursion of the principle):**
After the Pool capstone, recursed the same move one level out. Swarm/ProjectilePool were
reaching-through targets: callers did `state.swarm.pool.begin()` etc. Added forwarding
`begin()`/`end()` (+const) and `activeCount()` on Swarm/ProjectilePool that delegate to the
inner pool — Law of Demeter, callers talk only to the immediate object. Then dropped every
`.pool` from callers (last straggler scene_gameplay.cpp:108). With no external reach-in left,
made the data private: `struct Swarm` -> `class Swarm`, and put `pool`/`direction`/`speed`
under `private:` (ProjectilePool kept `struct` + explicit `private:` — same effect, access is
default-only). Grep confirmed 0 external `.pool` / `direction` / `speed` access (the lone
`.speed = PLAYER_SPEED` hit is Player init, false positive). Builds, runs identical.
The point: encapsulation composes — Pool hides its array, Swarm/ProjectilePool hide their Pool;
no caller anywhere touches representation, only operations.

**Status:** Pool is now a clean, fully-encapsulated generic container (acquire / activeCount / eachActive
/ begin-end / operator[]; private items). The C++ idiomatic-refactor arc is essentially COMPLETE — 16
lessons: RAII, constexpr, range-for, const, entities, scene polymorphism, SceneManager, return-next,
SRP(Swarm/Pool/HUD), Tell-Don't-Ask, fn templates, class templates, lambdas, encapsulation. The lesson's
ask-block teased revisiting the repo's original RL-environment goal (currently out of scope per
[[MISSION.md]]) — that is a MISSION CHANGE: confirm with user and add a learning record before starting.
