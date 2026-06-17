#include "player.hpp"
#include "game.h"
#include "raymath.h"

void InitPlayer(GameState *state) {
  state->player = (Player){.pos = INITIAL_PLAYER_POS,
                           .radius = PLAYER_RADIUS,
                           .speed = PLAYER_SPEED,
                           .dir = {0.0f, 0.0f},
                           .health = PLAYER_HEALTH};
}

void UpdatePlayer(GameState *state, float dt) {
  float radius = state->player.radius;
  float speed = state->player.speed;
  state->player.dir = Vector2Zero();
  Vector2 *playerPos = &state->player.pos;
  Vector2 *playerDir = &state->player.dir;

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    playerDir->x -= 1.0f;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    playerDir->x += 1.0f;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    playerDir->y -= 1.0f;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    playerDir->y += 1.0f;

  *playerDir = Vector2Normalize(*playerDir);
  *playerPos = Vector2Add(*playerPos, Vector2Scale(*playerDir, speed * dt));
  *playerPos =
      Vector2Clamp(*playerPos, (Vector2){radius, radius},
                   (Vector2){windowSize.x - radius, windowSize.y - radius});
};


void DrawPlayer(GameState *state) {
  DrawOffset(state->resources.playerTexture, state->player.pos, WHITE);
  DrawCircleLinesV(state->player.pos, state->player.radius, RED);
};

void PlayerShoot(GameState *state) {
  if (!IsKeyPressed(KEY_SPACE))
    return;

  PlaySound(state->resources.laserSound);
  FOR_EACH_PROJECTILE(bullet, state->bullets) {
    if (bullet->active)
      continue;

    bullet->active = true;
    bullet->pos = state->player.pos;
    bullet->dir = (Vector2){0.0f, -1.0f};
    bullet->speed = 500.0f;
    bullet->radius = 5.0f;
    break;
  }
}
