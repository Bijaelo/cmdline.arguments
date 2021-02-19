

#include "parser/pfunc/pfunc.h"


namespace cmdline_arguments::parser{

  // Calling operator() with only type specifier returns whether the type can
  // be used as input.
  // Maybe I'll actually change the template, so that it can "store" values.
  // That way it would itself be a container for
  template<typename T>
  bool parser_Function::operator()(){
    return false;
  };
  template<>
  bool parser_Function::operator()<individual_input>(){
    return true;
  };
  template<>
  bool parser_Function::operator()<vector_input>(){
    return true;
  };
  template<>
  bool parser_Function::operator()<list_input>(){
    return true;
  };


}
