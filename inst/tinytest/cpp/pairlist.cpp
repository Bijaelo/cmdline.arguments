

// Testing the overloaded pairlist definition in <cmd_args/RcppApi/pairlist.h>

// [[Rcpp::depends(cmdline.arguments)]]
// [[Rcpp::plugins("cpp2a")]]

#include <cmd_args/utils/ArgumentList.h> // Includes pairlist

// [[Rcpp::export]]
inline SEXP test_pairlist(SEXP x, SEXP fun){
  cmd_args::utils::ArgumentList argList(x);
  Rcpp::Function Fun(fun);
  return Fun(argList);
}
