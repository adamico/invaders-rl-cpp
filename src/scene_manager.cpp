#include "scene_manager.h"

SceneManager::SceneManager(std::unique_ptr<Scene> startingScene,
                           GameState& state)
    : current_(std::move(startingScene)) {
  current_->enter(state);
};

void SceneManager::tick(GameState& state, float deltaTime) {
  if (auto next = current_->update(state, deltaTime)) {
    switchTo(std::move(next), state);
  }

  current_->draw(state);
}

void SceneManager::switchTo(std::unique_ptr<Scene> nextScene,
                            GameState& state) {
  current_ = std::move(nextScene);
  current_->enter(state);
}
