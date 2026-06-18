#include "projectile_pool.h"

void ProjectilePool::reset() {
  for (auto& projectile : pool) {
    projectile = {.active = false};
  }
};

bool ProjectilePool::fire(Vector2 from, Vector2 dir) {
  Projectile* projectile = pool.acquire();
  if (!projectile) return false;

  projectile->spawn(from, dir);
  return true;
};

void ProjectilePool::update(float deltaTime) {
  pool.eachActive(
      [&](Projectile& projectile) { projectile.update(deltaTime); });
}

void ProjectilePool::draw(const Texture2D& texture) const {
  pool.eachActive(
      [&](const Projectile& projectile) { projectile.draw(texture); });
}
