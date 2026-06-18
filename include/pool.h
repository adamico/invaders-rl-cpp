#pragma once

#include <array>
#include <cstddef>

template <typename T, std::size_t N> class Pool {
public:
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

  template <typename F> void eachActive(F fn) const {
    for (const T& item : items)
      if (item.active) fn(item);
  }

  template <typename F> void eachActive(F fn) {
    for (T& item : items)
      if (item.active) fn(item);
  }

  auto begin() { return items.begin(); }
  auto end() { return items.end(); }

  auto begin() const { return items.begin(); }
  auto end() const { return items.end(); }

  T& operator[](std::size_t index) { return items[index]; }
  const T& operator[](std::size_t index) const { return items[index]; }

private:
  std::array<T, N> items{};
};
