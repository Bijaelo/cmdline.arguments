

#include "parser/argument/raw_argument.h"

namespace cmdline_arguments::parser::argument{
  raw_argument::raw_argument(string _type, string _name, vector<string> _data)
    : type(convert_type(_type, _name)), name(_name), data(_data)
  {}
  raw_argument::raw_argument(input_types _type, string _name, vector<string> _data)
    : type(_type), name(_name), data(_data)
  {}

  vector<string> raw_argument::operator()() const{
    return this -> data;
  }
  void raw_argument::operator+=(const string rhs){
    this -> add(rhs);
  }
  void raw_argument::operator+=(const vector<string> rhs){
    this -> add(rhs);
  }
  void raw_argument::add(string rhs){
    (this -> data).push_back(rhs);
  }
  void raw_argument::add(vector<string> rhs){
    (this -> data).insert(this -> data.end(), rhs.begin(), rhs.end());

  }
  Shield<SEXP> raw_argument::digest() const{
    List o;
    // Just a quick wrapper for data output.
    // We always return a list, so parse_func can use VECTOR_ELT(vec, 0)
    // to extract the element.
    switch(this -> type){
      case individual_input_type:
        o = wrap(this -> data);
        break;
      case vector_input_type:
        o = List::create(wrap(this -> data));
        break;
      case list_input_type:
        List x(wrap(this -> data));
        o = List::create(x);
        break;
    }
    return Shield<SEXP>(o);
  }
}
