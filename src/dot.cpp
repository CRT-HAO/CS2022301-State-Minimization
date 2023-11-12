#include "dot.hpp"
#include "utilities.hpp"

using namespace std;

void Dot::addTerm(std::string from_state, std::string to_state,
                  int present_state, int next_state) {
  for (auto &t : this->terms) {
    if (t.from_state == from_state && t.to_state == to_state) {
      t.states.push_back({present_state, next_state});
      return;
    }
  }

  Term term(from_state, to_state);
  term.from_state = from_state;
  term.to_state = to_state;
  term.states.push_back({present_state, next_state});
  this->terms.push_back(term);
}

void Dot::exports(std::ostream &out) const {
  out << "digraph STG {" << endl;
  out << '\t' << "rankdir=LR;" << endl;
  out << endl;

  out << '\t' << "INIT [shape=point];" << endl;
  for (const string &s : this->states)
    out << '\t' << s << " [label=\"" << s << "\"];" << endl;
  out << endl;

  out << '\t' << "INIT -> " << this->start_state << ";" << endl;
  for (const Term &t : this->terms) {
    out << '\t' << t.from_state << " -> " << t.to_state;
    out << " [";
    out << "label=\"";
    for (auto it = t.states.begin(); it != t.states.end(); ++it) {
      out << Utilities::decimalToBinary(it->present_state, this->inputs_num)
          << "/"
          << Utilities::decimalToBinary(it->next_state, this->outputs_num);
      if (it != t.states.end() - 1)
        out << ", ";
    }
    out << "\"];" << endl;
  }

  out << "}" << endl;
}