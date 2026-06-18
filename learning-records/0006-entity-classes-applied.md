# Entity classes done: Player/Enemy/Projectile own update()/draw(); swarm logic kept free

User converted all three entities from passive structs + free functions into structs with member
functions, tracer-bullet style (Projectile first, then Player, then Enemy). Builds and plays.

**Concepts demonstrated:**
- Member functions co-locate data + behaviour; bare member names refer to the implicit `this` object.
- `draw(const Texture2D&) const` const methods pair with `const Entity&` draw loops (lesson 4 payoff).
- **Encapsulation is per-concern all-or-nothing:** initially `spawn` set only pos/active; corrected to
  own all fire state. Then user *independently* generalized `spawn(Vector2 from, Vector2 dir = {0,-1})`
  with a defaulted direction to anticipate enemy fire (reasonable, low-risk anticipation vs YAGNI).
- Discovered `this->dir = dir` to disambiguate a parameter shadowing a member.
- **C.4 judgment — not everything should be a method:** correctly kept swarm edge-detection /
  direction-flip / game-over in the free `UpdateEnemies` (needs whole array + shared
  `enemyDirection`), exposing only per-enemy `moveHorizontally`/`moveVertically` as methods. This is
  the key skill of the lesson and the user reasoned it out.
- Cleaned consistency: added `Enemy::draw` to match the other two; unified screen-size source on the
  `windowSize` constexpr everywhere (resolved the long-standing mixed `windowSize`/`GetScreenWidth`
  deferred tidy-up).

**Implications:** User is fluent in basic class design and the "member vs free function" judgment.
Ready for the final roadmap item: **scene polymorphism** — replace the `GameScene` enum + `switch` in
main with a `Scene` base class + virtual `update()/draw()` (introduces inheritance, virtual, and
likely `std::unique_ptr` / move — the move semantics deferred since lesson 1). See [[NOTES.md]].
