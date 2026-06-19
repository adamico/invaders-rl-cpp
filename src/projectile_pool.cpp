#include "projectile_pool.h"

void ProjectilePool::reset(ProjectileSpec spec) {
  this->spec = spec;
  for (auto& projectile : pool) {
    projectile = {.active = false};
  }
};

bool ProjectilePool::fire(Vector2 from) {
  Projectile* projectile = pool.acquire();
  if (!projectile) return false;

  projectile->spawn(from, spec);
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
