#include "enemy.hpp"
#include "game.h"
#include "raymath.h"

#define ENEMY_RADIUS 15.0f
#define ENEMY_SCORE_VALUE 100

void Enemy::Init(Vector2 pos) {
  this->pos = pos;
  radius = ENEMY_RADIUS;
  scoreValue = ENEMY_SCORE_VALUE;
  active = true;
}

bool Enemy::WillHitEdge(Vector2 dir) {
  if (!active)
    return false;

  int rightEdge = GetScreenWidth() - ENEMY_RADIUS;
  int leftEdge = ENEMY_RADIUS;

  bool willHitRightEdge = (pos.x + radius) >= rightEdge && dir.x > 0;
  bool willHitLeftEdge = (pos.x - radius) <= leftEdge && dir.x < 0;

  return willHitRightEdge || willHitLeftEdge;
}

void Enemy::MoveDown(float amount) {
  pos.y += amount;
}

void Enemy::Update(Vector2 dir, float speed, float dt) {
  if (!active)
    return;

  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
}

void Enemy::Draw(GameState *state) {
  if (!active)
    return;

  DrawOffset(state->resources.enemyTexture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}
