
// [[Rcpp::depends(cmdline.arguments)]]
// [[Rcpp::plugins("cpp2a")]]

#include <Rcpp.h>
#include <cmd_args/utils/sugar.h>
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
bool allin_SEXPSEXP(const SEXP& x, const SEXP& table){
  return cmd_args::utils::sugar::allin(x, table);
}

// [[Rcpp::export]]
bool samelengths_NumericVector(const NumericVector& x){
  return cmd_args::utils::sugar::same_lengths(x);
}

// [[Rcpp::export]]
bool samelengths_SEXP(const SEXP& x){
  return cmd_args::utils::sugar::same_lengths(x);
}

// [[Rcpp::export]]
bool test_is_StringOrChar(const SEXP& x){
  return cmd_args::utils::sugar::is_StringOrChar(x);
}

// [[Rcpp::export]]
bool test_is_StringOrCharAndNotNA(const SEXP& x){
  return cmd_args::utils::sugar::is_StringOrCharAndNotNA(x);
}

// [[Rcpp::export]]
bool sexp_isnull(const SEXP& x){
  return cmd_args::utils::sugar::cmd_ISNULL(x);
}

