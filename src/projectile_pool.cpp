#include "projectile_pool.h"

constexpr float PROJECTILE_SPEED = 500.0f;
constexpr float PROJECTILE_RADIUS = 5.0f;

void ProjectilePool::reset() {
  for (auto& projectile : projectiles) {
    projectile = {.active = false};
  }
};

bool ProjectilePool::fire(Vector2 from, Vector2 dir) {
  for (auto& projectile : projectiles) {
    if (!projectile.active) {
      projectile.pos = from;
      projectile.dir = dir;
      projectile.speed = PROJECTILE_SPEED;
      projectile.radius = PROJECTILE_RADIUS;
      projectile.active = true;
      return true;
    }
  }
  return false;
};

void ProjectilePool::update(float deltaTime) {
  for (auto& projectile : projectiles) {
    projectile.update(deltaTime);
  }
}

void ProjectilePool::draw(const Texture2D& texture) const {
  for (auto& projectile : projectiles) {
    projectile.draw(texture);
  }
}
