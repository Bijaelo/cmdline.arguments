
// [[Rcpp::depends(cmdline.arguments)]]

#include <Rcpp.h>
#include <cmd_args/sugar.h>
using namespace Rcpp;


//  This test covers all standard Rcpp::Vector types
// [[Rcpp::export]]
bool anyDuplicated_NumericVector(const NumericVector& x){
  return cmd_args::utils::sugar::anyDuplicated(x);
}

// [[Rcpp::export]]
bool anyDuplicated_SEXP(const SEXP& x){
  return cmd_args::utils::sugar::anyDuplicated(x);
}

// [[Rcpp::export]]
bool allin_NumericNumeric(const NumericVector& x, const NumericVector& table){
  return cmd_args::utils::sugar::allin(x, table);
}

// [[Rcpp::export]]
bool allin_NumericInteger(const NumericVector& x, const IntegerVector& table){
  return cmd_args::utils::sugar::allin(x, table);
}

// [[Rcpp::export]]
bool allin_SEXPSEXP(const SEXP& x, const SEXP& table){
  return cmd_args::utils::sugar::allin(x, table);
}

// [[Rcpp::export]]
bool allin_SEXPINTEGER(const SEXP& x, const IntegerVector& table){
  return cmd_args::utils::sugar::allin(x, table);
}

// [[Rcpp::export]]
bool samelengths_NumericVector(const NumericVector& x){
  return cmd_args::utils::sugar::samelength(x);
}

// [[Rcpp::export]]
bool samelengths_SEXP(const SEXP& x){
  return cmd_args::utils::sugar::samelength(x);
}

// [[Rcpp::export]]
bool test_is_StringOrChar(const SEXP& x){
  return cmd_args::utils::sugar::is_StringOrChar(x);
}

// [[Rcpp::export]]
bool test_test_is_StringOrCharAndNotNA(const SEXP& x){
  return cmd_args::utils::sugar::test_is_StringOrCharAndNotNA(x);
}

// [[Rcpp::export]]
bool sexp_isnull(const SEXP& x){
  return cmd_args::utils::sugar::sexp_isnull(x);
}

