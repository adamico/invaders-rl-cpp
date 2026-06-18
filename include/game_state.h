#pragma once
#include "projectile_pool.h"
#include "swarm.h"

struct GameState {
  Player player;
  ProjectilePool projectilePool;
  Swarm swarm;
  bool victory;
  GameResources resources;
  int score;
};

void centerText(const char* text, int yPos, int fontSize, Color textColor);
void initGameplay(GameState* state);
