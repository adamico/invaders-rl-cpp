#include "scene_gameover.h"

void SceneGameover::update(GameState& state, float deltaTime) {
  if (IsKeyPressed(KEY_ENTER)) {
    state.currentScene = GameScene::TITLE;
    InitGameplay(&state);
  }
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
  CenterText(gameOverText, windowSize.y / 2, 50, textColor);
  CenterText("Press Enter to go to Title", windowSize.y / 2 + 100, 20,
             textColor);
  EndDrawing();
}
