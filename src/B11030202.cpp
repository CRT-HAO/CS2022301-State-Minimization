#include "debug.hpp"
#include "dot.hpp"
#include "state_minimization.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

using namespace std;

extern "C" {
EMSCRIPTEN_KEEPALIVE int add(int a, int b) { return a + b; }
EMSCRIPTEN_KEEPALIVE int sub(int a, int b) { return a - b; }

EMSCRIPTEN_KEEPALIVE const char *run(const char *input) {
  // cout << input << endl;

  stringstream ss(input);

  Kiss input_kiss;
  input_kiss.parse(ss);

  StateMinimization mini;

  try {
    mini.build_table(input_kiss);
  } catch (const char *msg) {
    out << "Error: " << msg << endl;
    return "Error";
  }

  mini.build_implication_table();

  out << "## Implication table" << endl;
  mini.printImplicationTable(out);

  out << "## Table" << endl;
  mini.printTable(out);

  mini.minimize();

  out << "## Minimized" << endl;
  out << "### Implication table" << endl;
  mini.printImplicationTable(out);
  out << "### Table" << endl;
  mini.printTable(out);

  Kiss output_kiss;
  output_kiss.inputs_num = input_kiss.inputs_num;
  output_kiss.outputs_num = input_kiss.outputs_num;
  output_kiss.states_num = 0;
  output_kiss.reset_state_var = input_kiss.reset_state_var;
  for (const auto &s : mini.table) {
    if (!s.removed)
      output_kiss.states_num += 1;
  }
  output_kiss.terms_num = output_kiss.states_num * mini.inputs_num;
  size_t i = 0;
  for (const auto &s : mini.table) {
    if (!s.removed) {

      for (size_t j = 0; j < mini.inputs_num; ++j) {
        output_kiss.terms.push_back({int(j), mini.states[i],
                                     mini.states[s.next_states[j]],
                                     s.output[j]});
      }
    }
    ++i;
  }

  stringstream oss_kiss;
  output_kiss.exports(oss_kiss);

  out << "## Output kiss" << endl;
  out << "```kiss" << endl << oss_kiss.str() << "```" << endl;

  Dot output_dot;
  output_dot.inputs_num = output_kiss.inputs_num;
  output_dot.outputs_num = output_kiss.outputs_num;
  output_dot.start_state = input_kiss.reset_state_var;
  size_t j = 0;
  for (const auto &s : mini.table) {
    if (!s.removed) {
      output_dot.states.push_back(mini.states[j]);

      for (size_t k = 0; k < mini.inputs_num; ++k) {
        output_dot.addTerm(mini.states[j], mini.states[s.next_states[k]], k,
                           s.output[k]);
      }
    }
    ++j;
  }

  stringstream oss_dot;
  output_dot.exports(oss_dot);
  out << "## Output dot" << endl;
  out << "```dot" << endl << oss_dot.str() << "```" << endl;

  cout << "Success" << endl;

  char *result = new char[out.str().length() + 1];
  strcpy(result, out.str().c_str());

  return result;
}
}

void printUsage(const char *executable_name) {
  cout << "Usage: " << executable_name
       << " <input_kiss> <output_kiss> <output_dot> [--debug]" << endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  if (argc >= 5)
    debug = (strcmp(argv[4], "--debug") == 0);

  const char *input_kiss_file = argv[1];
  const char *output_kiss_file = argv[2];
  const char *output_dot_file = argv[3];

  ifstream ifs;
  ifs.open(input_kiss_file);
  if (!ifs.is_open()) {
    cout << "Error: cannot open input kiss file" << endl;
    return 1;
  }

  ofstream ofs_kiss;
  ofs_kiss.open(output_kiss_file);
  if (!ofs_kiss.is_open()) {
    cout << "Error: cannot open output kiss file" << endl;
    return 1;
  }

  ofstream ofs_dot;
  ofs_dot.open(output_dot_file);
  if (!ofs_dot.is_open()) {
    cout << "Error: cannot open output dot file" << endl;
    return 1;
  }

  Kiss input_kiss;
  input_kiss.parse(ifs);

  StateMinimization mini;

  try {
    mini.build_table(input_kiss);
  } catch (const char *msg) {
    cout << "Error: " << msg << endl;
    return 1;
  }

  mini.build_implication_table();

  if (debug) {
    cout << "## Implication table" << endl;
    mini.printImplicationTable(cout);

    cout << "## Table" << endl;
    mini.printTable(cout);
  }

  mini.minimize();

  if (debug) {
    cout << "## Minimized" << endl;
    cout << "### Implication table" << endl;
    mini.printImplicationTable(cout);
    cout << "### Table" << endl;
    mini.printTable(cout);
  }

  Kiss output_kiss;
  output_kiss.inputs_num = input_kiss.inputs_num;
  output_kiss.outputs_num = input_kiss.outputs_num;
  output_kiss.states_num = 0;
  output_kiss.reset_state_var = input_kiss.reset_state_var;
  for (const auto &s : mini.table) {
    if (!s.removed)
      output_kiss.states_num += 1;
  }
  output_kiss.terms_num = output_kiss.states_num * mini.inputs_num;
  size_t i = 0;
  for (const auto &s : mini.table) {
    if (!s.removed) {

      for (size_t j = 0; j < mini.inputs_num; ++j) {
        output_kiss.terms.push_back({int(j), mini.states[i],
                                     mini.states[s.next_states[j]],
                                     s.output[j]});
      }
    }
    ++i;
  }

  output_kiss.exports(ofs_kiss);

  Dot output_dot;
  output_dot.inputs_num = output_kiss.inputs_num;
  output_dot.outputs_num = output_kiss.outputs_num;
  output_dot.start_state = input_kiss.reset_state_var;
  size_t j = 0;
  for (const auto &s : mini.table) {
    if (!s.removed) {
      output_dot.states.push_back(mini.states[j]);

      for (size_t k = 0; k < mini.inputs_num; ++k) {
        output_dot.addTerm(mini.states[j], mini.states[s.next_states[k]], k,
                           s.output[k]);
      }
    }
    ++j;
  }

  output_dot.exports(ofs_dot);

  cout << "Success" << endl;

  return 0;
}