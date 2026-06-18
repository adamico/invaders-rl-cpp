#pragma once

#include "game.h"
#include "pool.h"
#include "raylib.h"

constexpr int MAX_PROJECTILES = 100;

struct ProjectilePool {
  Pool<Projectile, MAX_PROJECTILES> pool;

  void reset();
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  bool fire(Vector2 from, Vector2 dir = {0.0f, -1.0f});
  int activeCount() const { return pool.activeCount(); }
};
