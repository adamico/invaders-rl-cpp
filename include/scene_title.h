#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "game.h"
#include "scene.h"

struct SceneTitle : Scene {
  void update(GameState& state, float deltaTime) override;
  void draw(const GameState& state) const override;
};

#endif
