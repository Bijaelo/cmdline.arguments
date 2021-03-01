


#ifndef cmdline_pfunc
#define cmdline_pfunc


#include "utils/traits/traits.h"
#include "utils/ArgumentList.h"
//#include "utils/converters/converters.h"
using std::string;
using Rcpp::List, Rcpp::stop, Rcpp::Function;
using cmdline_arguments::utils::ArgumentList;

namespace cmdline_arguments::parser{


  // The following 3 implementation have the same constructors (shame on me)
  // but the actual method needs to be different
  class parserFunction{
  public:

    // function, Name
    template<typename T>
    parserFunction(T fun):
      func(fun), args(List::create()){ }
    // function, name, arguments
    template<typename T, typename U>
    parserFunction(T fun, U _args):
      func(fun), args(_args){ }

    template<typename T>
    SEXP operator()(const T& rawArgs){
      List ra = as<List>(wrap(rawArgs));
      ra = appendList(ra);
      ArgumentList fullArgs(ra);
      return (this -> func)(fullArgs);
    };
    // This does not seem to be doing anything?
    template<typename T>
    SEXP operator()(){
      ArgumentList fullArgs(this -> args);
      return (this -> func)(fullArgs);
    }

  private:
    const Function func;
    const List args;

    // 2 functions for appending. But one will be taken care of by operator()(const T&)
    inline void insertList(List::iterator& start, CharacterVector::iterator& Nstart,
                           const List& data, R_xlen_t& n) const {
      SEXP names = data.names();
      if(Rf_length(names) == 0){
        for(R_xlen_t i = 0; i < n; i++, start++)
          *start = data[i];
        Nstart = Nstart + n;
      }else{
        for(R_xlen_t i = 0; i < n; i++, start++, Nstart++){
          *start = data[i];
          SEXP namei = STRING_ELT(names, i);
          if(namei != R_NilValue){
            *Nstart = namei;
          }
        }
      }
    }
    // Not how we'll do it, as we have args stored.
    inline SEXP appendList(const List& rawArgs) const {
      BEGIN_RCPP
      R_xlen_t rawSize = rawArgs.size(),
        argsSize = (this -> args).size(),
        totalSize = rawSize + argsSize;
      List res(totalSize);
      CharacterVector resNames(totalSize);
      auto it = res.begin();
      auto ni = resNames.begin();
      insertList(it, ni, rawArgs, rawSize);
      insertList(it, ni, this -> args, argsSize);
      res.attr("names") = resNames;
      return res;
      END_RCPP
    }

  };
}


#endif
