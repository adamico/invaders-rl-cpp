#include "player.h"

#include "canvas.h"
#include "raymath.h"
#include <cmath>

// public methods

void Player::reset() {
  speed = PLAYER_SPEED;
  radius = PLAYER_RADIUS;
  lives = PLAYER_LIVES;
  respawn();
  invulnerableTimer = 0.0f;
}

void Player::update(float dt) {
  handleInput();
  move(dt);

  if (invulnerableTimer > 0.0f) invulnerableTimer -= dt;
};

void Player::draw(const Texture2D& texture) const {
  if (!isVulnerable() && fmodf(invulnerableTimer, 0.2f) > 0.1f) return;

  DrawTextureV(texture, Vector2Add(pos, CANVAS_OFFSET), WHITE);
  DrawCircleLinesV(pos, radius, RED); // hitbox
};

void Player::takeDamage() {
  if (!isVulnerable()) return;

  loseLife();
}

// private methods

void Player::move(float dt) {
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  pos = Vector2Clamp(
      pos, (Vector2){radius, radius},
      (Vector2){GetScreenWidth() - radius, GetScreenHeight() - radius});
}

void Player::handleInput() {
  dir = Vector2Zero();

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dir.x -= 1.0f;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dir.x += 1.0f;

  dir = Vector2Normalize(dir);
}

void Player::respawn() {
  pos = {GetScreenWidth() / 2.0f, GetScreenHeight() - PLAYER_BOTTOM_MARGIN};
  dir = {0.0f, 0.0f};
  invulnerableTimer = INVULNERABILITY_TIME;
}

void Player::loseLife() {
  --lives;
  if (lives > 0) respawn();
}
