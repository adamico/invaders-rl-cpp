#pragma once

#include "raylib.h"
template <typename A, typename B> bool overlaps(const A& a, const B& b) {
  return CheckCollisionCircles(a.pos, a.radius, b.pos, b.radius);
}
