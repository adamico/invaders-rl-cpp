#pragma once

#include "raylib.h"
#include "raymath.h"

class Enemy {
public:
  Enemy() {};
  void Init(Vector2 pos);
  void Update(Vector2 dir, float speed,float dt);
  void Draw(struct GameState* state);
  void MoveDown(float amount);
  bool WillHitEdge(Vector2 dir);

  Vector2 pos = Vector2Zero();
  float radius = 0.0f;
  int scoreValue = 0;
  bool active = false;
};
