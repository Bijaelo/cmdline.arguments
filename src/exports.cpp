#include <Rcpp.h>

#include "utils/matching.h"
//#include <arguments.h> // currently non-existent
//#include <parser.h> // currently non-existent

extern "C" {

  // using section

  // std
  using std::vector;
  using std::string;

  // Rcpp
  using Rcpp::RObject;
  using Rcpp::wrap;
  using Rcpp::as;
  using Rcpp::IntegerVector;
  using Rcpp::LogicalVector;
  using Rcpp::traits::input_parameter;

  // Rcpp type definitions (for simpler casting with Rcpp::as<T>(x))
  typedef input_parameter<const string&>::type r_string;
  typedef input_parameter<const vector<string>&>::type r_v_string;
  typedef input_parameter<const vector<vector<string>>&>::type r_v_v_string;
  typedef input_parameter<const long&>::type r_long;
  typedef input_parameter<const vector<long>&>::type r_v_long;
  typedef input_parameter<const R_xlen_t&>::type r_R_xlen_t;
  typedef input_parameter<const vector<R_xlen_t>&>::type r_v_R_xlen_t;
  typedef input_parameter<const IntegerVector&>::type r_IntegerVector;
  typedef input_parameter<const LogicalVector&>::type r_LogicalVector;


  // cmdline_arguments/Utils
  using cmdline_arguments::utils::matching::find_args_c;
  using cmdline_arguments::utils::matching::find_args_single_c;
  using cmdline_arguments::utils::matching::substring_c;
  using cmdline_arguments::utils::matching::match_args_c;


  // R-exposed function definitions:

  SEXP _find_args_c(SEXP _x, SEXP _argS, SEXP _argL){
  BEGIN_RCPP
    RObject ret =
      wrap(find_args_c(as<std::vector<std::string>>(_x),
                       as<std::string>(_argS),
                       as<std::string>(_argL)));
    return ret;
  END_RCPP
  }
  SEXP _find_args_single_c(SEXP _x, SEXP _arg){
  BEGIN_RCPP
    RObject ret =
      wrap(
        find_args_single_c(as<r_v_string>(_x),
                           as<r_string>(_arg))
        );
    return ret;
  END_RCPP
  }

  SEXP _substring_c(SEXP _x, SEXP _l){
  BEGIN_RCPP
    RObject ret =
      substring_c(as<r_v_string>(_x), as<r_v_long>(_l));
    return ret;
  END_RCPP
  }

  SEXP _match_args_c(SEXP _cmdArgs,
                    SEXP _cmdArgsPosition,
                    SEXP _cmdArgsNextPositions,
                    SEXP _parserArgs,
                    SEXP _parserArgsOrder,
                    SEXP _parserArgsRequired,
                    SEXP _helpArg){
  BEGIN_RCPP
    RObject ret =
      match_args_c(as<r_v_string>(_cmdArgs),
                   as<r_v_R_xlen_t>(_cmdArgsPosition),
                   as<r_v_R_xlen_t>(_cmdArgsNextPositions),
                   as<r_v_v_string>(_parserArgs),
                   as<r_IntegerVector>(_parserArgsOrder),
                   as<r_LogicalVector>(_parserArgsRequired),
                   as<r_string>(_helpArg));
    return ret;
  END_RCPP
  }
}


#include <R_ext/Rdynload.h>

// Create a list of pointers to be exported to R
static const R_CallMethodDef CallEntries[] = {

  // <utils/matching.h>
  {"_find_args_c", (DL_FUNC) _find_args_c, 3},
  {"_find_args_single_c", (DL_FUNC) _find_args_single_c, 2},
  {"_substring_c", (DL_FUNC) &_substring_c, 2},
  {"_match_args_c", (DL_FUNC) &_match_args_c, 7},
  {NULL, NULL, 0}
};

// Register the pointers as Dynamic Symbols for R.
extern "C" void R_init_cmdline_arguments(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, (Rboolean)FALSE);
};
