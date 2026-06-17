#pragma once

#include "raylib.h"

class Enemy {
public:
  Enemy();
  void Init(Vector2 startPos);
  void Update(Vector2 dir, float speed,float dt);
  void Draw(struct GameState* state);
  void MoveDown(float amount);
  bool WillHitEdge(Vector2 dir);

  Vector2 pos;
  float radius;
  bool active;
  int scoreValue;
};
