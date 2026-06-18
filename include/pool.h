#pragma once

#include <array>
#include <cstddef>

template <typename T, std::size_t N> struct Pool {
  std::array<T, N> items{};

  T* acquire() {
    for (T& item : items)
      if (!item.active) return &item;
    return nullptr;
  }

  int activeCount() const {
    int count = 0;
    for (const T& item : items)
      if (item.active) ++count;
    return count;
  }
};
