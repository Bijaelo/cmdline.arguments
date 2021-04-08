

#ifndef test_include
#define test_include
#include <Rcpp.h>
using namespace Rcpp;
namespace cmd_args{
  SEXP hello_world(SEXP x){
    NumericVector xx = as<NumericVector>(x);
    Rcout << xx << "\n"
          << "Hello world\n";
    return wrap(xx);
  }
}
#endif
