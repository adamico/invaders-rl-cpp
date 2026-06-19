#include "player.h"
#include "canvas.h"
#include "raymath.h"

// public methods

void Player::reset() {
  speed = PLAYER_SPEED;
  radius = PLAYER_RADIUS;
  lives = PLAYER_LIVES;
  respawn();
  invulnerableTimer = 0.0f;
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

  if (invulnerableTimer > 0.0f) invulnerableTimer -= dt;
};

void Player::draw(const Texture2D& texture) const {
  if (!isVulnerable() && fmodf(invulnerableTimer, 0.2f) > 0.1f) return;

  DrawTextureV(texture, Vector2Add(pos, CANVAS_OFFSET), WHITE);
  DrawCircleLinesV(pos, radius, RED); // hitbox
};

void Player::takeDamage(int amount) {
  if (!isVulnerable()) return;

  loseLife();
}

// private methods

void Player::respawn() {
  pos = {GetScreenWidth() / 2.0f, GetScreenHeight() - (PLAYER_RADIUS * 4)};
  dir = {0.0f, 0.0f};
  invulnerableTimer = INVULNERABILITY_TIME;
}

void Player::loseLife() {
  --lives;
  if (lives > 0) respawn();
}
