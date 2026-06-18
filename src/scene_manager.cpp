#include "scene_manager.h"

SceneManager::SceneManager(std::unique_ptr<Scene> startingScene)
    : current_(std::move(startingScene)) {};

void SceneManager::tick(GameState& state, float deltaTime) {
  if (auto next = current_->update(state, deltaTime)) {
    current_ = std::move(next);
  }

  current_->draw(state);
}
