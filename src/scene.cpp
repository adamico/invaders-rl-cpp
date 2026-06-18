#include "scene.h"
#include "scene_gameover.h"
#include "scene_gameplay.h"
#include "scene_title.h"
#include <memory>

std::unique_ptr<Scene> makeScene(GameScene which) {
  switch (which) {
  case GameScene::TITLE:
    return std::make_unique<SceneTitle>();
  case GameScene::GAMEPLAY:
    return std::make_unique<SceneGameplay>();
  case GameScene::GAMEOVER:
    return std::make_unique<SceneGameover>();
  }
  return nullptr;
}
