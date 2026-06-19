#pragma once

#include "pool.h"
#include "projectile.h"
#include "raylib.h"

constexpr int MAX_PROJECTILES = 100;
constexpr float PROJECTILE_SPEED = 500.0f;

struct ProjectilePool {
public:
  void reset(ProjectileSpec spec);
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  bool fire(Vector2 from);
  int activeCount() const { return pool.activeCount(); }

  auto begin() { return pool.begin(); }
  auto end() { return pool.end(); }
  auto begin() const { return pool.begin(); }
  auto end() const { return pool.end(); }

private:
  Pool<Projectile, MAX_PROJECTILES> pool;
  ProjectileSpec spec{};
};
