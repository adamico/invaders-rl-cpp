# C++20 concepts: naming the contract templates already assumed

User completed lesson 0017. Replaced bare `typename` duck-typing with named, checked constraints on the
two template surfaces built in L13/L14:
- `collision.h`: `concept Collidable = requires(const T t){ {t.pos}->std::convertible_to<Vector2>;
  {t.radius}->std::convertible_to<float>; };` → `template <Collidable A, Collidable B> overlaps`.
- `pool.h`: `concept Poolable = requires(T t){ {t.active}->std::convertible_to<bool>; };` →
  `template <Poolable T, std::size_t N> class Pool`.
Builds, plays identical (concepts are compile-time only — zero codegen change).

**Concepts (NEW mechanic):**
- `concept Name = requires(...) {...};` = a named constexpr-bool predicate over a type.
- requires-expression: simple requirement `{ expr };` (must compile) vs compound
  `{ expr } -> Constraint;` (must compile AND result type satisfies the trailing *concept*, not a plain
  type — `std::convertible_to<float>`, not `float`). The `->` is NOT a return-type arrow.
- `template <Collidable A>` reads as constrained `typename`; equivalent spellings = trailing
  `requires` clause or abbreviated `Collidable auto&`. Constrains TYPE params only; `std::size_t N`
  (non-type) stays bare.
- Payoff is diagnostics + self-documentation, not behaviour: wrong type rejected AT THE BOUNDARY
  ("constraint not satisfied") instead of a deep error inside the body. Same machine code.

**Toolchain friction (the real content of this session — all environment, not language):**
1. c++17 → c++20 needed in THREE places, not one: `Makefile` (the actual build), `.clangd`
   (`CompileFlags.Add: -std=c++20` — this is what unblocked the editor; clangd ignores the Makefile),
   and `compile_commands.json`. clangd "doesn't recognize concept" = stale `-std`, not a code bug.
2. `compile_commands.json` was a hand-written stale snapshot: 3 entries, one referencing a long-deleted
   `player.cpp` (entities moved to headers in L5). Explained the compilation-database role + clangd's
   flag inference for unlisted files. User regenerated with `bear -- make` → 8 real TUs, correct flags.
   (Caveat noted: bear only records what actually compiles, so it needs a clean/dirty build to capture.)
3. IWYU: code compiled WITHOUT `#include <concepts>` via a transitive include (`<array>` pulled it in on
   libc++). Taught this is incidental/fragile, not guaranteed — "include what you use." User added the
   explicit include to both headers.

**Calibration:** user is now spotting environment/tooling smells unprompted (noticed the 3-file DB,
questioned the missing include) — same instinct previously shown for code smells. Strong IWYU + build-
hygiene intuition. The toolchain detour was arguably more instructive than the language feature here.

**Status:** Concepts in. The C++ idiomatic surface is now genuinely exhausted across 17 lessons (RAII,
constexpr, range-for, const, entities, scene polymorphism, SceneManager, return-next, SRP, Tell-Don't-Ask,
fn templates, class templates, lambdas, encapsulation, concepts). The lesson's ask-block + NOTES both
point at the decision: revisit the repo's original RL-environment goal ([[MISSION.md]]) — a MISSION CHANGE
requiring explicit user confirmation + a fresh learning record before starting. Awaiting user direction.
