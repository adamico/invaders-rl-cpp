#pragma once

#include "raylib.h"

constexpr Vector2 windowSize = {1280, 720};

struct GameResources {
  Texture2D playerTexture;
  Texture2D enemyTexture;
  Texture2D laserTexture;
  Sound laserSound;
  Sound explosionSound;

  GameResources();
  ~GameResources();

  GameResources(const GameResources&) = delete;
  GameResources& operator=(const GameResources&) = delete;
};
