#pragma once

#include <raylib.h>

struct ProjectileSpec {
  float radius = 5;
  float speed = 500.0f;
  Vector2 dir = {0.0f, -1.0f};
  bool flipVertical = false;
};

struct Projectile {
  Vector2 pos{};
  Vector2 dir{};
  float speed{};
  float radius{};
  bool flipVertical{};
  bool active{};

  void spawn(Vector2 from, const ProjectileSpec& spec);
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void deactivate() { active = false; };
};
