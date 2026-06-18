#ifndef SCENE_GAMEOVER_H
#define SCENE_GAMEOVER_H

#include "game.h"
#include "scene.h"

struct SceneGameover : Scene {
  void update(GameState& state, float deltaTime) override;
  void draw(const GameState& state) const override;
};

#endif
