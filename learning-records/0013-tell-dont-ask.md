# Tell-Don't-Ask: moved the swarm's activeCount invariant home

User completed lesson 0012. **User spotted the smell themselves** ("the two collision check functions
are sending red code smell signals") — a strong sign they're now reading their own code critically.
Added `Swarm::deactivate(Enemy&)` (idempotent `if (!enemy.active) return;` guard, flips flag + decrements
activeCount atomically) and `Projectile::deactivate()` (inline in game.h). Both collision free-functions
now call `state.swarm.deactivate(enemy)` / `bullet.deactivate()` instead of poking
`enemy.active = false; state.swarm.activeCount--`. Verified: zero `activeCount` *writes* outside
swarm.cpp; the only external reference left is the victory-check *read* (`activeCount <= 0`), which is
fine. Builds, plays.

**Concepts demonstrated:**
- Invariant ownership (C.2): the activeCount==#active rule now lives only where activeCount lives.
- Tell, Don't Ask / Law of Demeter: collision code tells the swarm what happened rather than reaching
  two dots deep to mutate its internals.
- The C.4 discriminator sharpened: `deactivate` is a MEMBER (maintains the type's invariant) while the
  collision CHECK stays FREE (mediates two types, owns neither invariant) — same rule, opposite
  conclusions for two functions in the same file. User now applies this fluently.
- Idempotent guard as invariant protection (prevents double-decrement drift).

**Side threads this session (good engagement, conceptual not code):**
- Asked whether trivial method bodies belong in headers — taught in-class definition = implicitly
  inline (ODR-safe in headers), convention: one line/no deps -> header, else .cpp. (Their
  `Projectile::deactivate(){...}` is the header-inline case; rest of structs keep bodies in .cpp.)
- clang-format `AllowShortIfStatementsOnSingleLine` — preserves one-line ifs but never collapses
  multi-line ones (relevant: their .clang-format already allows one-line ifs, see commit 5a5af9a).

**Calibration (carry forward):** SRP + member-vs-free judgment is solid; user is self-diagnosing smells.
Pure-repetition lessons add little — prioritise genuinely NEW mechanics next.

**Named-but-deferred smells (still open, candidate next arcs):** split collision *check* from *effect*
(name lies / testability); duplicated `if (!active) continue;` across pools -> an "iterate active only"
helper, which is the natural on-ramp to the **templates** arc (Pool<T> / active-iteration) — the biggest
remaining gap per the learner profile (templates). Also still open: make swarm/pool members private;
per-scene state / pause overlay (scene stack). Confirm with user before starting.
