#include "hud.h"

constexpr int HUD_FONT_SIZE = 20;
constexpr Color HUD_TEXT_COLOR = WHITE;
constexpr int HUD_MARGIN = 20;

void drawHud(int lives, int score) {
  Vector2 scorePosition = {HUD_MARGIN, HUD_MARGIN};
  Vector2 healthPosition = {HUD_MARGIN,
                            (float)GetScreenHeight() - HUD_MARGIN * 2};

  DrawText(TextFormat("Health: %i", lives), healthPosition.x, healthPosition.y,
           HUD_FONT_SIZE, HUD_TEXT_COLOR);
  DrawText(TextFormat("Score: %i", score), scorePosition.x, scorePosition.y,
           HUD_FONT_SIZE, HUD_TEXT_COLOR);
}
