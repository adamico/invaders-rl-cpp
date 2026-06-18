#pragma once
#include "projectile_pool.h"
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

void centerText(const char* text, int yPos, int fontSize, Color textColor);
void initGameplay(GameState* state);
