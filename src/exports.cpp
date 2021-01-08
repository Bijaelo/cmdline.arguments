#include <Rcpp.h>

#include "utils/matching.h"
#include "parser/argument/argument.h"
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
  // Note: for some string types it does not seem like casting from SEXP to for
  // example std::string works with just "as<std::string>()". It does however work
  // if we use the long input_parameter declaration (so maybe my type is slightly off).
  typedef input_parameter<const string&>::type c_r_string;
  typedef input_parameter<const vector<string>&>::type c_r_v_string;
  typedef input_parameter<const vector<vector<string>>&>::type c_r_v_v_string;
  typedef input_parameter<const long&>::type c_r_long;
  typedef input_parameter<const vector<long>&>::type c_r_v_long;
  typedef input_parameter<const R_xlen_t&>::type c_r_R_xlen_t;
  typedef input_parameter<const vector<R_xlen_t>&>::type c_r_v_R_xlen_t;
  typedef input_parameter<const IntegerVector&>::type c_r_IntegerVector;
  typedef input_parameter<const LogicalVector&>::type c_r_LogicalVector;


  // cmdline_arguments/Utils
  using cmdline_arguments::utils::matching::find_args_c;
  using cmdline_arguments::utils::matching::find_args_single_c;
  using cmdline_arguments::utils::matching::substring_c;
  using cmdline_arguments::utils::matching::match_args_c;
  using cmdline_arguments::parser::arguments::argument;


  // R-exposed function definitions:

  SEXP _find_args_c(SEXP _x, SEXP _argS, SEXP _argL){
  BEGIN_RCPP
    RObject ret =
      wrap(find_args_c(as<vector<string>>(_x),
                       as<string>(_argS),
                       as<string>(_argL)));
    return ret;
  END_RCPP
  }
  SEXP _find_args_single_c(SEXP _x, SEXP _arg){
  BEGIN_RCPP
    RObject ret =
      wrap(
        find_args_single_c(as<vector<string>>(_x),
                           as<string>(_arg))
        );
    return ret;
  END_RCPP
  }

  SEXP _substring_c(SEXP _x, SEXP _l){
  BEGIN_RCPP
    RObject ret =
      substring_c(as<vector<string>>(_x), as<vector<long>>(_l));
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
      match_args_c(as<vector<string>>(_cmdArgs),
                   as<vector<R_xlen_t>>(_cmdArgsPosition),
                   as<vector<R_xlen_t>>(_cmdArgsNextPositions),
                   as<vector<vector<string>>>(_parserArgs),
                   as<IntegerVector>(_parserArgsOrder),
                   as<LogicalVector>(_parserArgsRequired),
                   as<string>(_helpArg));
    return ret;
  END_RCPP
  }
  SEXP gen_class(SEXP _sarg = R_NilValue,
                 SEXP _larg = R_NilValue,
                 SEXP _name = R_NilValue,
                 SEXP _narg = R_NilValue){
  BEGIN_RCPP
    XPtr<argument> ptr(new argument(as<c_r_string>(_sarg),
                                    as<c_r_string>(_larg),
                                    as<c_r_string>(_name),
                                    as<c_r_string>(_narg)));
    return wrap(ptr);
  END_RCPP
  }
  SEXP getsarg(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return (_ptr -> getsarg());
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
  {"gen_class", (DL_FUNC) &gen_class, 1},
  {"getsarg", (DL_FUNC) &getsarg, 1},
  {NULL, NULL, 0}
};

// Register the pointers as Dynamic Symbols for R.
extern "C" void R_init_cmdline_arguments(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, (Rboolean)FALSE);
}
