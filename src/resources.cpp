#include "resources.h"

#include "raylib.h"

GameResources::GameResources() {
  playerTexture = LoadTexture("player.png");
  enemyTexture = LoadTexture("enemy.png");
  laserTexture = LoadTexture("laser.png");
  laserSound = LoadSound("laser.wav");
  explosionSound = LoadSound("explosion.wav");
}

GameResources::~GameResources() {
  UnloadTexture(playerTexture);
  UnloadTexture(enemyTexture);
  UnloadTexture(laserTexture);
  UnloadSound(laserSound);
  UnloadSound(explosionSound);
}
