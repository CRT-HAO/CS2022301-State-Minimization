#pragma once

#include <cstddef>
#include <string>
#include <vector>

class State {
public:
  std::vector<size_t> next_states;
  std::vector<int> output;
  bool removed;

  State() : removed(false) {}
};