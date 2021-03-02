

#include "parser/argument/raw_argument.h"

namespace cmdline_arguments::parser::argument{
  raw_argument::raw_argument(string _type, string _name,
                             vector<string> _data,
                             Rcpp::String _outputName = R_NilValue)
    : type(convert_type(_type, _name)), name(_name), data(_data), outputName(_outputName)
  {}
  raw_argument::raw_argument(input_types _type, string _name,
                             vector<string> _data,
                             Rcpp::String _outputName = R_NilValue)
    : type(_type), name(_name), data(_data), outputName(_outputName)
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
  ArgumentList raw_argument::digest() const{
    List o;
    // Just a quick wrapper for data output.
    // We always return a list, so parse_func can use VECTOR_ELT(vec, 0)
    // to extract the element.
    switch(this -> type){
      case individual_input_type:
        o = wrap(this -> data);
        break;
      case vector_input_type:
        if(this -> outputName == R_NilValue)
            o = List::create(wrap(this -> data));
        else
          o = List::create(Rcpp::Named(this -> outputName) = wrap(this -> data));
        break;
      case list_input_type:
        List x(wrap(this -> data));
        if(this -> outputName == R_NilValue)
          o = List::create(x);
        else
          o = List::create(Rcpp::Named(this -> outputName) = x);
        break;
    }
    return ArgumentList(o);
  }

  // Implicit conversion
  raw_argument::operator ArgumentList() const{
    return digest();
  }
}

