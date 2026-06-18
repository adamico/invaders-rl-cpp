#include "scene_manager.h"

SceneManager::SceneManager(GameScene startScene)
    : current_(makeScene(startScene)), shown_(startScene) {}

void SceneManager::tick(GameState& state, float deltaTime) {
  if (state.currentScene != shown_) {
    current_ = makeScene(state.currentScene);
    shown_ = state.currentScene;
  }

  current_->update(state, deltaTime);
  current_->draw(state);
}
