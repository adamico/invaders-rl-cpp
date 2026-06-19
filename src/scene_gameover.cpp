#include "scene_gameover.h"

#include "scene_title.h"

std::unique_ptr<Scene> SceneGameover::update(GameState& state,
                                             float deltaTime) {
  if (IsKeyPressed(KEY_ENTER)) {
    return std::make_unique<SceneTitle>();
  }
  return nullptr;
}

void SceneGameover::draw(const GameState& state) const {
  BeginDrawing();
  ClearBackground(BLACK);
  const char* gameOverText = "Game Over";
  Color textColor = RED;

  if (state.victory) {
    gameOverText = "You Win!";
    textColor = GREEN;
  }
  centerText(gameOverText, windowSize.y / 2, 50, textColor);
  centerText("Press Enter to go to Title", windowSize.y / 2 + 100, 20,
             textColor);
  EndDrawing();
}
