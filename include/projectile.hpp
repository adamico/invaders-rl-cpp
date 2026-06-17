#pragma once

#include "raylib.h"
#include "raymath.h"

class Projectile {
public:
  Projectile() {};
  void Init(Vector2 startPos, Vector2 dir);
  void Update(float dt);
  void Draw(struct GameState* state);

  Vector2 pos = Vector2Zero();
  Vector2 dir = Vector2Zero();
  float speed = 0.0f;
  float radius = 0.0f;
  bool active = false;
};
