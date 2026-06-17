#pragma once

#include "raylib.h"

#define INITIAL_PLAYER_POS                                                     \
  (Vector2) { windowSize.x / 2, windowSize.y - (PLAYER_RADIUS * 4) }
#define PLAYER_RADIUS 12.5f
#define PLAYER_SPEED 300.0f
#define PLAYER_HEALTH 5

// Player definition
typedef struct Player {
  Vector2 pos;
  float radius;
  float speed;
  Vector2 dir;
  int health;
} Player;

struct GameState;

void InitPlayer(GameState* state);
void UpdatePlayer(GameState* state, float dt);
void DrawPlayer(GameState* state);
void PlayerShoot(GameState* state);
