#include "scene_title.h"
#include "raylib.h"
#include "scene_gameplay.h"

const char* TITLE_TEXT = "Invaders RL";
const char* SUBTITLE_TEXT = "Press Space to Start";

void CenterText(const char* text, int yPos, int fontSize, Color textColor) {
  int textSize = MeasureText(text, fontSize);
  DrawText(text, (int)windowSize.x / 2 - textSize / 2, yPos, fontSize,
           textColor);
}

std::unique_ptr<Scene> SceneTitle::update(GameState& state, float) {
  if (IsKeyPressed(KEY_SPACE))
    return std::make_unique<SceneGameplay>();
  return nullptr;
}

void SceneTitle::draw(const GameState& state) const {
  BeginDrawing();
  ClearBackground(BLACK);
  CenterText(TITLE_TEXT, windowSize.y / 2, 40, WHITE);
  CenterText(SUBTITLE_TEXT, windowSize.y / 2 + 100, 20, WHITE);
  EndDrawing();
}
