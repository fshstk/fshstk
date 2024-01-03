#pragma once
#include <vector>

class IndexedVector
{
public:
  void resize(size_t newSize) { data.resize(newSize); }

  auto get() const -> float { return data[index]; }
  void add(float val) { data[index] += val; }
  void set(float val) { data[index] = val; }
  void increment() { index = (index + 1) % data.size(); }
  void clear() { std::fill(data.begin(), data.end(), 0.0f); }

private:
  size_t index;
  std::vector<float> data;
};
