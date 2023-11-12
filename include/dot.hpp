#pragma once

#include <ostream>
#include <string>
#include <vector>

class Dot {
public:
  class Term {
  public:
    typedef struct {
      int present_state;
      int next_state;
    } State;

    std::string from_state;
    std::string to_state;
    std::vector<State> states;

  public:
    Term(std::string from_state = "", std::string to_state = "")
        : from_state(from_state), to_state(to_state) {}
  };

public:
  std::vector<std::string> states;
  std::vector<Term> terms;
  std::string start_state;
  size_t inputs_num;
  size_t outputs_num;

public:
  Dot() {}

public:
  void addTerm(std::string from_state, std::string to_state, int present_state,
               int next_state);

public:
  void exports(std::ostream &) const;
};