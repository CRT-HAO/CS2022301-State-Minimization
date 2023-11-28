#pragma once

#include "kiss.hpp"
#include "state.hpp"

#include <string>
#include <vector>

class StateMinimization {
private:
  class Implication {
  public:
    bool incompatible;
    std::vector<std::vector<size_t>> next_states;

  public:
    Implication(bool imcompatible = false) : incompatible(imcompatible) {}
  };

public:
  size_t inputs_num;
  std::string reset_state;
  std::vector<std::string> states;
  std::vector<State> table;
  std::vector<std::vector<Implication>> implication_table;

public:
  StateMinimization() : inputs_num(0) {}

public:
  size_t findStateIndex(const std::string &) const;
  size_t addState(const std::string &, size_t inputs_num = 2);

public:
  void build_table(const Kiss &);
  void build_implication_table();
  void minimize();

public:
  void printTable(std::ostream &) const;
  void printImplicationTable(std::ostream &) const;
};