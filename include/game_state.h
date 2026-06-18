#pragma once
#include "swarm.h"

struct GameState {
  Player player;
  std::array<Projectile, MAX_PROJECTILES> bullets;
  Swarm swarm;
  bool victory;
  GameResources resources;
  int score;
};

void CenterText(const char* text, int yPos, int fontSize, Color textColor);
void InitGameplay(GameState* state);
