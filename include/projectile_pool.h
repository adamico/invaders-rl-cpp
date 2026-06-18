#pragma once

#include "game.h"
#include "raylib.h"
#include <array>

constexpr int MAX_PROJECTILES = 100;

struct ProjectilePool {
  std::array<Projectile, MAX_PROJECTILES> projectiles{};

  void reset();
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  bool fire(Vector2 from, Vector2 dir = {0.0f, -1.0f});
};
