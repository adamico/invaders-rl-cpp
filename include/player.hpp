#pragma once

#include "raylib.h"

#define PLAYER_RADIUS 12.5f
#define PLAYER_SPEED 300.0f
#define PLAYER_HEALTH 5

class Player {
public:
  Player() {};
  void Init(Vector2 startPos);
  void Update(struct GameState *state, float dt);
  void Draw(struct GameState *state);
  void Shoot(struct GameState *state);

  Vector2 pos;
  float radius;
  float speed;
  Vector2 dir;
  int health;
};
