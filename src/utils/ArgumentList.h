


// ArgumentList class
// Simple container for special handling of arguments (in Rcpp::Function::operator()(T& t1))

#ifndef cmdline_arguments_ArgumentList_H
#define cmdline_arguments_ArgumentList_H

//#include <Rcpp.h> // <== always included in other files.
using namespace Rcpp;
namespace cmdline_arguments::utils{
  class ArgumentList{
  public:
    template<typename T>
    ArgumentList(T& _data) :data(as<List>(_data)){};

    template<typename T>
    ArgumentList() : data(List::create()){};

    List& operator()(){
      return this -> data;
    }
    List data; // Probably shouldm'y br
    // For automatic conversion to Rcpp::List.
    operator List(){
      return this -> data;
    }
    void operator=(List data){
      this -> data = data;
    }

    // Iterators are taken directly from Rcpp::List
    using Iter = List::iterator;
    using CIter = List::const_iterator;
    Iter begin(){return data.begin();}
    Iter end(){return data.end();}
    CIter cbegin(){return data.cbegin();}
    CIter cend(){return data.cend();}

  };

}


// pairlist(ArgumentList) has to be loaded here. Otherwise we throw wrap error.
// (cannot convert ArgumentList to SEXP)
#include "RcppApi/pairlist.h"

#endif
