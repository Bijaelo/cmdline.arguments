#include <Rcpp.h>

#include "utils/matching.h"
#include "parser/argument/argument.h"
#include <Rinternals.h>
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
  // I know these work as intended, because they worked as intended in the test package
  // So the error is somewhere in the constructor, the class or in a function called during initialization.
  // It must be in the constructor as we are working with the const verb.
  SEXP createArgument(SEXP _sflag = R_NilValue,
                      SEXP _lflag = R_NilValue,
                      SEXP _name = R_NilValue,
                      SEXP _narg = R_NilValue,
                      SEXP _flag = R_NilValue,
                      SEXP _required = R_NilValue){
  BEGIN_RCPP
    /* narg needs a few extra conversion checks.
     * just in case we were provided with an integer/double value, we'll convert
     * it to an appropriate character.
     * To avoid compiler specific behaviour we'll use std::floor
     * in the case of double values.
     */
    // Flags should be single boolean values and not missing
    // FIX ME: check for missing values! (Error: no function defined for R_isnancpp)
    // onces fixed remove the check in argument.R
    /*
    if(!((TYPEOF(_flag) == LGLSXP && Rf_length(_flag) == 1)||
       (TYPEOF(_required) == LGLSXP && Rf_length(_required) == 1)))
      stop("flag and required must be either TRUE or FALSE");
    */
    XPtr<argument> ptr(new argument(as<string>(_sflag),
                                    as<string>(_lflag),
                                    as<string>(_name),
                                    as<string>(_narg),
                                    // convert SEXP to boolean.
                                    Rf_asLogical(_flag) != 0,
                                    Rf_asLogical(_required) != 0));
    return wrap(ptr);
  END_RCPP
  }
  SEXP createArgumentS(SEXP _flagg = R_NilValue,
                      SEXP _name = R_NilValue,
                      SEXP _narg = R_NilValue,
                      SEXP _flag = R_NilValue,
                      SEXP _required = R_NilValue){
    BEGIN_RCPP
    XPtr<argument> ptr(new argument(as<string>(_flagg),
                                    as<string>(_name),
                                    as<string>(_narg),
                                    // _flag == 0 and _required == 1..
                                    (bool)(Rf_asLogical(_flag) != 0),
                                    (bool)(Rf_asLogical(_required) != 0)));
    return wrap(ptr);
    END_RCPP
  }
  SEXP getsflag(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return (_ptr -> getsflag());
  }
  SEXP getlflag(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return (_ptr -> getlflag());
  }
  SEXP getflags(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return _ptr -> getflags();
  }
  SEXP getnarg(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return _ptr -> getnarg();
  }
  SEXP getnargparsed(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return _ptr -> getnargparsed();
  }
  SEXP setUnparsedArgs(SEXP _ptr, SEXP _unparsedargs){
    XPtr<argument> ptr(_ptr);
    vector<string> unparsedargs = Rcpp::as<vector<string>>(_unparsedargs);
    // This try-catch seems necessary. The switch -- stop crashes the sessions without it.
    BEGIN_RCPP
    ptr -> setUnparsedArgs(unparsedargs);
    VOID_END_RCPP
    return R_NilValue;
  }
  SEXP getname(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return _ptr -> getname();
  }
  SEXP isflag(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return _ptr -> isflag();
  }
  SEXP isrequired(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    return _ptr -> isrequired();
  }
  SEXP demandReady(SEXP ptr){
    XPtr<argument> _ptr(ptr);
    _ptr -> demandReady();
    return R_NilValue;
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
  {"_createArgument", (DL_FUNC) &createArgument, 6},
  {"_createArgumentS", (DL_FUNC) &createArgumentS, 5},
  {"_getsflag", (DL_FUNC) &getsflag, 1},
  {"_getlflag", (DL_FUNC) &getlflag, 1},
  {"_getflags", (DL_FUNC) &getflags, 1},
  {"_getnarg", (DL_FUNC) &getnarg, 1},
  {"_getname", (DL_FUNC) &getname, 1},
  {"_isflag", (DL_FUNC) &isflag, 1},
  {"_isrequired", (DL_FUNC) &isrequired, 1},
  {"_getnargparsed", (DL_FUNC) &getnargparsed, 1},
  {"_setUnparsedArgs", (DL_FUNC) &setUnparsedArgs, 2},
  {"_demandReady", (DL_FUNC) &demandReady, 1},
  {NULL, NULL, 0}
};

// Register the pointers as Dynamic Symbols for R.
extern "C" void R_init_cmdline_arguments(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, (Rboolean)FALSE);
}
