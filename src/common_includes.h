

/* This file exists only to avoid double inclusion.
 *
 * Because of the nature of cmd_coerce.h where I redefine quite a few things,
 * it is important that cmd_coerce.h is included prior to Rinternals.h
 * Otherwise a long series of very confusing errors.
 *
 * For the final release a deep dive into the code, to uncover "how" to
 * get access to these elements will likely be necessary, but for now, I'll
 * keep these redefinitions and just use this header for top inclusion to ensure
 * Rinternals is not included first.
 *
 */

#ifndef cmd_Rinternals_shielded_include__
#define cmd_Rinternals_shielded_include__

#include <R.h>
// Rcpp
#include <Rcpp.h>

// using Rcpp::as,
//   Rcpp::wrap,
//   Rcpp::RObject,
//   Rcpp::List,
//   Rcpp::Environment,
//   Rcpp::IntegerVector,
//   Rcpp::LogicalVector,
//   Rcpp::traits::input_parameter,
//   Rcpp::Rcout;

// standard library
#include <string>
#include <vector>

// using std::vector,
//   std::vector;

#include "utils/cmd_coerce.h"

#include <Rinternals.h>


#endif
