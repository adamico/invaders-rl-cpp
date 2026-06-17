#pragma once

#include "raylib.h"

#define INITIAL_PLAYER_POS                                                     \
  (Vector2) { windowSize.x / 2, windowSize.y - (PLAYER_RADIUS * 4) }
#define PLAYER_RADIUS 12.5f
#define PLAYER_SPEED 300.0f
#define PLAYER_HEALTH 5

struct GameState;

// Player definition
class Player {
public:
  Player();
  void Update(struct GameState* state, float dt);
  void Draw(struct GameState* state);
  void Shoot(struct GameState* state);
  Vector2 pos;
  float radius;
  float speed;
  Vector2 dir;
  int health;
};
