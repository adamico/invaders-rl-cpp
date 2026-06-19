#pragma once

#include "scene.h"

struct SceneTitle : Scene {
  std::unique_ptr<Scene> update(GameState& state, float deltaTime) override;
  void draw(const GameState& state) const override;
};

void initGameplay(GameState* state);
void centerText(const char* text, int yPos, int fontSize, Color textColor);
