#include "state_minimization.hpp"
#include "debug.hpp"
#include "utilities.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

size_t StateMinimization::findStateIndex(const std::string &name) const {
  auto it = find(this->states.begin(), this->states.end(), name);

  if (it != this->states.end())
    return it - this->states.begin();

  return SIZE_T_MAX;
}

size_t StateMinimization::addState(const std::string &name, size_t inputs_num) {
  this->states.push_back(name);

  this->table.push_back({});
  (this->table.end() - 1)->next_states.assign(inputs_num, SIZE_T_MAX);
  (this->table.end() - 1)->output.assign(inputs_num, -1);

  return this->states.size() - 1;
}

void StateMinimization::build_table(const Kiss &kiss) {
  if (kiss.outputs_num != 1)
    throw "outputs number not supported, outputs number must be 1";

  this->inputs_num = (1 << kiss.inputs_num); // 2 ^ kiss.inputs_num

  this->states.clear();

  this->table.clear();

  for (const Kiss::Term &term : kiss.terms) {
    size_t state_index = this->findStateIndex(term.state);
    if (state_index == SIZE_T_MAX)
      state_index = this->addState(term.state, this->inputs_num);

    size_t next_state_index = this->findStateIndex(term.next_state);
    if (next_state_index == SIZE_T_MAX)
      next_state_index = this->addState(term.next_state, this->inputs_num);

    this->table[state_index].next_states[term.input] = next_state_index;
    this->table[state_index].output[term.input] = term.output;
  }

  for (const auto &s : this->table) {
    for (size_t i = 0; i < kiss.inputs_num; i++) {
      if (s.next_states[i] == SIZE_T_MAX || s.output[i] == -1)
        throw "build tabl from kiss file failed";
    }
  }
}

void StateMinimization::build_implication_table() {
  if (this->table.size() == 0)
    throw "tabls is empty, you should build table first";

  const size_t vars_num = this->table.size();

  // assign the 2d vector
  this->implication_table.assign(vars_num, vector<Implication>(vars_num));

  for (size_t i = 1; i < vars_num; ++i) {
    for (size_t j = 0; j < i; ++j) {
      Implication &block = this->implication_table[i][j];

      State &state_a = this->table[i];
      State &state_b = this->table[j];

      block.next_states.resize(this->inputs_num);

      // set block to incompatible if any output is different in this pair
      for (size_t k = 0; k < this->inputs_num; ++k) {
        block.incompatible |= state_a.output[k] != state_b.output[k];

        block.next_states[k].push_back(state_a.next_states[k]);
        block.next_states[k].push_back(state_b.next_states[k]);
        sort(block.next_states[k].begin(), block.next_states[k].end(),
             std::greater<size_t>());
      }
    }
  }
}

void StateMinimization::minimize() {
  const size_t vars_num = this->table.size();

  // check all implications is incompatible
  bool anyIncompatible;
  size_t step = 0;
  do {
    anyIncompatible = false;

    for (size_t i = 1; i < vars_num; ++i) {
      for (size_t j = 0; j < i; ++j) {
        Implication &block = this->implication_table[i][j];

        if (block.incompatible)
          // skip if the block is incompatible
          continue;

        for (size_t k = 0; k < this->inputs_num; ++k) {
          const auto &next_index = block.next_states[k];

          const Implication &next_states_pair =
              this->implication_table[next_index[0]][next_index[1]];

          if (next_states_pair.incompatible) {
            block.incompatible = true;
            anyIncompatible |= true;
          }
        }

        if (debug) {
          std::cout << "## Minimize step " << step << std::endl;
          this->printImplicationTable(std::cout);
          ++step;
        }
      }
    }
  } while (anyIncompatible);

  // merge the states
  for (size_t i = 1; i < vars_num; ++i) {
    for (size_t j = 0; j < i; ++j) {
      Implication &block = this->implication_table[i][j];

      if (block.incompatible)
        // skip if the block is incompatible
        continue;

      size_t leave = j;
      size_t remove = i;

      for (auto &state : this->table) {
        for (auto &next : state.next_states) {
          if (next == remove)
            next = leave;
        }
      }

      this->table[remove].removed = true;
    }
  }
}

void StateMinimization::printTable(std::ostream &out) const {
  out << "| var | ";
  for (size_t i = 0; i < this->inputs_num; ++i) {
    out << Utilities::decimalToBinary(i, log2(this->inputs_num)) << " next | "
        << Utilities::decimalToBinary(i, log2(this->inputs_num))
        << " output | ";
  }
  out << endl;
  out << "| --- | ";
  for (size_t i = 0; i < this->inputs_num; ++i) {
    out << " --- |  --- | ";
  }
  out << endl;

  size_t i = 0;
  for (const auto &s : this->table) {
    if (s.removed)
      continue;

    out << "| " << this->states[i] << " | ";
    for (size_t i = 0; i < this->inputs_num; ++i) {
      out << this->states[s.next_states[i]] << " | " << s.output[i] << " | ";
    }
    out << endl;

    ++i;
  }
}

void StateMinimization::printImplicationTable(std::ostream &out) const {
  out << "|   | ";
  for (size_t i = 0; i < this->table.size(); ++i)
    out << this->states[i] << " | ";
  out << endl;
  out << "| ";
  for (size_t i = 0; i < this->table.size() + 1; ++i)
    out << "--- | ";
  out << endl;

  size_t i = 0;
  for (const auto &ii : this->implication_table) {
    out << "| " << this->states[i] << " | ";
    for (const auto &jj : ii) {
      if (jj.incompatible) {
        out << "X";
      } else {
        for (const auto &kk : jj.next_states) {
          for (const auto &s : kk)
            out << this->states[s] << '-';

          out << " / ";
        }
      }
      out << " | ";
    }
    out << endl;

    ++i;
  }
}