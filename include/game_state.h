#pragma once

#include "bunker.h"
#include "player.h"
#include "projectile_pool.h"
#include "resources.h"
#include "swarm.h"

struct GameState {
  Player player;
  ProjectilePool projectilePool;
  ProjectilePool enemyProjectilePool;
  std::array<Bunker, 4> bunkers;
  Swarm swarm;
  bool victory;
  GameResources resources;
  int score;
  float enemyFireCooldown;
};
