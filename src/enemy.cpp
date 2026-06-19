#include "enemy.h"

#include "canvas.h"
#include <raymath.h>

void Enemy::moveHorizontally(Vector2 dir, float speed, float deltaTime) {
  pos = Vector2Add(pos, Vector2Scale(dir, speed * deltaTime));
}

void Enemy::moveVertically(float amount) { pos.y += amount; }

void Enemy::draw(const Texture2D& texture) const {
  if (!active) return;
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}
