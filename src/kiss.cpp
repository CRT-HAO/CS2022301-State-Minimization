#include "kiss.hpp"
#include "state.hpp"
#include "utilities.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void Kiss::parse(std::istream &in) {
  bool start_kiss = false;
  for (string line; getline(in, line);) {
    stringstream ss(line);

    string command;
    ss >> command;

    if (command == ".start_kiss") {
      start_kiss = true;
      continue;
    }

    if (!start_kiss)
      continue;

    if (command == ".i")
      ss >> this->inputs_num;
    else if (command == ".o")
      ss >> this->outputs_num;
    else if (command == ".p")
      ss >> this->terms_num;
    else if (command == ".s")
      ss >> this->states_num;
    else if (command == ".r")
      ss >> this->reset_state_var;
    else if (command == ".end_kiss")
      return;
    else if (command.length() != 0) {
      int input = Utilities::binarytoDecimal(command);
      string var;
      string next_state_var;
      int output;
      ss >> var >> next_state_var >> output;

      this->terms.push_back({input, var, next_state_var, output});
    }
  }
}

void Kiss::exports(std::ostream &out) const {
  out << ".start_kiss" << endl;
  out << ".i " << this->inputs_num << endl;
  out << ".o " << this->outputs_num << endl;
  out << ".p " << this->terms_num << endl;
  out << ".s " << this->states_num << endl;
  out << ".r " << this->reset_state_var << endl;
  for (const Term &t : this->terms)
    out << Utilities::decimalToBinary(t.input, this->inputs_num) << " "
        << t.state << " " << t.next_state << " "
        << Utilities::decimalToBinary(t.output, this->outputs_num) << endl;
  out << ".end_kiss" << endl;
}
