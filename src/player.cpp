#include "player.h"
#include "canvas.h"
#include "raymath.h"

void Player::reset() {
  pos = {GetScreenWidth() / 2.0f, GetScreenHeight() - (PLAYER_RADIUS * 4)};
  dir = {0.0f, 0.0f};
  speed = PLAYER_SPEED;
  radius = PLAYER_RADIUS;
  hp = PLAYER_HEALTH;
}

void Player::update(float dt) {
  dir = Vector2Zero();

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dir.x -= 1.0f;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dir.x += 1.0f;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) dir.y -= 1.0f;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) dir.y += 1.0f;

  dir = Vector2Normalize(dir);
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  pos = Vector2Clamp(
      pos, (Vector2){radius, radius},
      (Vector2){GetScreenWidth() - radius, GetScreenHeight() - radius});
};

void Player::takeDamage(int amount) {
  hp -= amount;
  if (hp < 0) hp = 0;
}

void Player::die() { hp = 0; }

void Player::draw(const Texture2D& texture) const {
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
};
