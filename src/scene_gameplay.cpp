#include "raylib.h"
#include "raymath.h"
#include <stdarg.h>
#include <stdio.h>

#include "game.h"
#include "scene_gameplay.h"
#include "player.hpp"

#define INITIAL_PLAYER_POS                                                     \
  (Vector2) { GetScreenWidth() / 2.0f, GetScreenHeight() - (PLAYER_RADIUS * 4) }

#define MAX_ENEMIES_PER_ROW 11
#define ROW_PADDING 60
#define COL_PADDING 80

#define START_SWARM_POSITION                                                   \
  (Vector2){                                                                   \
      ((GetScreenWidth() - (MAX_ENEMIES_PER_ROW * COL_PADDING)) / 2.0f) +      \
          (COL_PADDING / 2.0f),                                                \
      100}

void InitGameplay(GameState *state) {
  GameResources resBackup = state->resources;
  *state = (GameState){};
  state->resources = resBackup;
  state->victory = false;
  state->player.Init(INITIAL_PLAYER_POS);
  state->swarm.Init(START_SWARM_POSITION);
  FOR_EACH_PROJECTILE(bullet, state->bullets) {
    bullet->active = false;
  }
}

void DrawOffset(Texture2D texture, Vector2 pos, Color tint) {
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, tint);
}

void CheckIfPlayerDied(GameState *state) {
  if (state->player.health <= 0)
    state->currentScene = GAMEOVER;
}

void CheckIfPlayerWon(GameState *state) {
  if (state->swarm.activeCount <= 0) {
    state->victory = true;
    state->currentScene = GAMEOVER;
  }
}

void UpdateGameplay(GameState *state, float dt) {
  // CheckIfPlayerDied(state);
  // CheckIfPlayerWon(state);
  state->player.Update(state, dt);
  state->swarm.Update(state->swarm.direction, state->swarm.speed, dt);
  state->player.Shoot(state);
  FOR_EACH_PROJECTILE(bullet, state->bullets) { bullet->Update(dt); }
  state->swarm.HandleCollisions(state);
}

void DrawGameplay(GameState *state) {
  BeginDrawing();

  // Setup the back buffer for drawing (clear color and depth buffers)
  ClearBackground(BLACK);

  int font_size = 20;

  state->player.Draw(state);
  state->swarm.Draw(state);
  FOR_EACH_PROJECTILE(bullet, state->bullets) { bullet->Draw(state); }

  DrawText(TextFormat("Health: %i", state->player.health), 20,
           GetScreenHeight() - 40, font_size, WHITE);

  DrawText(TextFormat("Score: %i", state->score), 20, 20, font_size, WHITE);
  EndDrawing();
}
