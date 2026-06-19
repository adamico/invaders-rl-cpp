#pragma once

#include "player.h"
#include "projectile_pool.h"
#include "resources.h"
#include "swarm.h"

struct GameState {
  Player player;
  ProjectilePool projectilePool;
  ProjectilePool enemyProjectilePool;
  Swarm swarm;
  bool victory;
  GameResources resources;
  int score;
  float enemyFireCooldown;
};
