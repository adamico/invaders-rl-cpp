#include "projectile.hpp"
#include "game.h"
#include "raylib.h"
#include "raymath.h"

void Projectile::Init(Vector2 startPos, Vector2 direction) {
  pos = startPos;
  dir = direction;
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
