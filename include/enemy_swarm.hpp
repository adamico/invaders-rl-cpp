#pragma once

#include "raylib.h"

#include "enemy.hpp"
#include "raymath.h"

#define MAX_ENEMIES 55
#define ENEMY_SPEED 50.0f

class EnemySwarm {
public:
  EnemySwarm() {};
  void Init(Vector2 gridStartPos);
  void Update(Vector2 dir, float speed, float dt);
  void Draw(struct GameState *state);
  void HandleCollisions(struct GameState *state);

  Vector2 direction = Vector2Zero();
  int speed = 0.0f;
  bool needToMoveDown = false;
  int activeCount = 0;

  Enemy enemies[MAX_ENEMIES];
};
