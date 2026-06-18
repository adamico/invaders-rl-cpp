#ifndef SCENE_GAMEPLAY_H
#define SCENE_GAMEPLAY_H

#include "game.h"
#include "scene.h"

struct SceneGameplay : Scene {
  void update(GameState& state, float dt) override;
  void draw(const GameState& state) const override;
};

#endif
