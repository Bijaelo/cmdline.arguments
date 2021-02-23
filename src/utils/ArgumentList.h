


// ArgumentList class
// Simple container for special handling of arguments (in Rcpp::Function::operator()(T& t1))

#ifndef cmdline_arguments_ArgumentList_H
#define cmdline_arguments_ArgumentList_H

#include <Rcpp.h>
using namespace Rcpp;
namespace cmdline_arguments::utils{
  class ArgumentList{
  public:
    template<typename T>
    ArgumentList(T& _data) :data(as<List>(_data)){};
    List& operator()(){
      return this -> data;
    }
    List data;
  };
}
#endif
