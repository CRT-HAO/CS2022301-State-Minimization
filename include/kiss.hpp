#pragma once

#include "state.hpp"

#include <string>
#include <vector>

class Kiss {
public:
  class Term {
  public:
    int input;
    std::string state;
    std::string next_state;
    int output;

  public:
    Term(int input = 0, std::string state = "", std::string next_state = "",
         int output = 0)
        : input(input), state(state), next_state(next_state), output(output) {}
  };

public:
  int inputs_num;
  int outputs_num;
  int terms_num;
  int states_num;
  std::string reset_state_var;
  std::vector<Term> terms;

public:
  Kiss() : inputs_num(0), outputs_num(0), terms_num(0), states_num(0) {}

public:
  void parse(std::istream &);
  void exports(std::ostream &) const;
};