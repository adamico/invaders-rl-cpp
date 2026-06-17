#include "projectile.hpp"
#include "game.h"
#include "raymath.h"

Projectile::Projectile() {
  pos = (Vector2){0.0f, 0.0f};
  dir = (Vector2){0.0f, 0.0f};
  speed = 500.0f;
  radius = 5.0f;
  active = true;
}

void Projectile::Update(float dt) {
  if (!active)
    return;

  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  active = pos.y > 0;
}

void Projectile::Draw(GameState* state) {
  if (!active)
    return;

  DrawOffset(state->resources.laserTexture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}
