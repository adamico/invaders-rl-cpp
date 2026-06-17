#include "projectile.hpp"
#include "game.h"
#include "raylib.h"
#include "raymath.h"



void Projectile::Init(Vector2 pos, Vector2 dir) {
  this->pos = pos;
  this->dir = dir;
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
