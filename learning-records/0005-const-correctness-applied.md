# const-correctness applied to draw path; learned const propagation + name mangling

User added `const` to read-only range-for loop variables and converted all four `Draw*` functions
(`DrawPlayer`, `DrawEnemies`, `DrawProjectiles`, `DrawGameplay`) to take `const GameState*`. Links
and runs clean; behaviour unchanged.

**Concepts demonstrated (learned by hitting the errors):**
- `const&` vs `&` in range-for now reliably chosen by read-vs-mutate intent.
- **const propagation:** a `const` function can only call other `const`-accepting functions, so
  `DrawGameplay` couldn't be const until `DrawPlayer` was — const flows outward from the leaves.
- **Name mangling / ODR:** changing the definition's signature to `const GameState*` but leaving the
  header declaration as `GameState*` produced an "Undefined symbols" linker error, because the two
  signatures mangle to different symbols. Fixed by syncing header declarations with definitions.
  User now understands that in C++ the parameter types are part of the linked symbol name.

**Implications:** User is ready for class methods — specifically `void draw() const` will now make
sense as "const method = the const-correct version of these draw functions." Outstanding tidy-up
(deferred): `UpdatePlayer`/`UpdateEnemies` still read the `windowSize` global while `Init*` use
`GetScreenWidth()` — mixed screen-size source, to reconcile when entities are introduced. Next:
lesson 5 — Player/Enemy/Projectile become classes owning update()/draw(). See [[NOTES.md]].
