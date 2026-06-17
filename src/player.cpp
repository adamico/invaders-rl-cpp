#include "player.hpp"
#include "game.h"
#include "raymath.h"

void Player::Init(Vector2 startPos) {
  pos = startPos;
  radius = PLAYER_RADIUS;
  speed = PLAYER_SPEED;
  dir = {0.0f, 0.0f};
  health = PLAYER_HEALTH;
}

void Player::Update(GameState *state, float dt) {
  dir = Vector2Zero();

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    dir.x -= 1.0f;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    dir.x += 1.0f;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    dir.y -= 1.0f;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    dir.y += 1.0f;

  dir = Vector2Normalize(dir);
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  pos = Vector2Clamp(pos, (Vector2){radius, radius},
                     (Vector2){GetScreenWidth() - radius, GetScreenHeight() - radius});
};

void Player::Draw(GameState *state) {
  DrawOffset(state->resources.playerTexture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
};

void Player::Shoot(GameState *state) {
  if (!IsKeyPressed(KEY_SPACE))
    return;

  PlaySound(state->resources.laserSound);
  FOR_EACH_PROJECTILE(bullet, state->bullets) {
    if (bullet->active)
      continue;

    bullet->Init(pos, (Vector2){0.0f, -1.0f});
    bullet->speed = 500.0f;
    bullet->radius = 5.0f;
    break;
  }
}
