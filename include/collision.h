#pragma once

#include "raylib.h"
#include <concepts>

template <typename T>
concept Collidable = requires(const T t) {
  { t.pos } -> std::convertible_to<Vector2>;
  { t.radius } -> std::convertible_to<float>;
};

template <Collidable A, Collidable B> bool overlaps(const A& a, const B& b) {
  return CheckCollisionCircles(a.pos, a.radius, b.pos, b.radius);
}
