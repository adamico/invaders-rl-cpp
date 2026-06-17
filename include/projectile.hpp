#pragma once

#include "raylib.h"

class Projectile {
public:
  Projectile() {};
  void Init(Vector2 startPos, Vector2 dir);
  void Update(float dt);
  void Draw(struct GameState* state);

  Vector2 pos;
  Vector2 dir;
  float speed;
  float radius;
  bool active;
};
