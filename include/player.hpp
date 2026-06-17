#pragma once

#include "raylib.h"
#include "raymath.h"

class Player {
public:
  Player() {};
  void Init(Vector2 startPos);

  void Update(struct GameState *state, float dt);
  void Draw(struct GameState *state);
  void Shoot(struct GameState *state);

  Vector2 pos = Vector2Zero();
  float radius = 0.0f;
  float speed = 0.0f;
  Vector2 dir = Vector2Zero();
  int health = 0;
  bool active = false;
};
