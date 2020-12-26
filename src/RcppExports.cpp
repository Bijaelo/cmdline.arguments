// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// find_args_c
IntegerVector find_args_c(const std::vector<std::string> x, const std::string argS, const std::string argL);
RcppExport SEXP _cmdline_arguments_find_args_c(SEXP xSEXP, SEXP argSSEXP, SEXP argLSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::traits::input_parameter< const std::vector<std::string> >::type x(xSEXP);
    Rcpp::traits::input_parameter< const std::string >::type argS(argSSEXP);
    Rcpp::traits::input_parameter< const std::string >::type argL(argLSEXP);
    rcpp_result_gen = Rcpp::wrap(find_args_c(x, argS, argL));
    return rcpp_result_gen;
END_RCPP
}
// find_args_single_c
IntegerVector find_args_single_c(const std::vector<std::string> x, const std::string arg);
RcppExport SEXP _cmdline_arguments_find_args_single_c(SEXP xSEXP, SEXP argSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::traits::input_parameter< const std::vector<std::string> >::type x(xSEXP);
    Rcpp::traits::input_parameter< const std::string >::type arg(argSEXP);
    rcpp_result_gen = Rcpp::wrap(find_args_single_c(x, arg));
    return rcpp_result_gen;
END_RCPP
}
// substring_c
CharacterVector substring_c(const std::vector<std::string> x, const std::vector<long> l);
RcppExport SEXP _cmdline_arguments_substring_c(SEXP xSEXP, SEXP lSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::traits::input_parameter< const std::vector<std::string> >::type x(xSEXP);
    Rcpp::traits::input_parameter< const std::vector<long> >::type l(lSEXP);
    rcpp_result_gen = Rcpp::wrap(substring_c(x, l));
    return rcpp_result_gen;
END_RCPP
}
// match_args_c
Rcpp::List match_args_c(const std::vector<std::string>& cmdArgs, const std::vector<R_xlen_t>& cmdArgsPositions, const std::vector<R_xlen_t>& cmdArgsNextPositions, const std::vector<std::vector<std::string>>& parserArgs, const IntegerVector& parserArgsOrder, const LogicalVector& parserArgsRequired, const std::string& helpArg);
RcppExport SEXP _cmdline_arguments_match_args_c(SEXP cmdArgsSEXP, SEXP cmdArgsPositionsSEXP, SEXP cmdArgsNextPositionsSEXP, SEXP parserArgsSEXP, SEXP parserArgsOrderSEXP, SEXP parserArgsRequiredSEXP, SEXP helpArgSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::traits::input_parameter< const std::vector<std::string>& >::type cmdArgs(cmdArgsSEXP);
    Rcpp::traits::input_parameter< const std::vector<R_xlen_t>& >::type cmdArgsPositions(cmdArgsPositionsSEXP);
    Rcpp::traits::input_parameter< const std::vector<R_xlen_t>& >::type cmdArgsNextPositions(cmdArgsNextPositionsSEXP);
    Rcpp::traits::input_parameter< const std::vector<std::vector<std::string>>& >::type parserArgs(parserArgsSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type parserArgsOrder(parserArgsOrderSEXP);
    Rcpp::traits::input_parameter< const LogicalVector& >::type parserArgsRequired(parserArgsRequiredSEXP);
    Rcpp::traits::input_parameter< const std::string& >::type helpArg(helpArgSEXP);
    rcpp_result_gen = Rcpp::wrap(match_args_c(cmdArgs, cmdArgsPositions, cmdArgsNextPositions, parserArgs, parserArgsOrder, parserArgsRequired, helpArg));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_cmdline_arguments_find_args_c", (DL_FUNC) &_cmdline_arguments_find_args_c, 3},
    {"_cmdline_arguments_find_args_single_c", (DL_FUNC) &_cmdline_arguments_find_args_single_c, 2},
    {"_cmdline_arguments_substring_c", (DL_FUNC) &_cmdline_arguments_substring_c, 2},
    {"_cmdline_arguments_match_args_c", (DL_FUNC) &_cmdline_arguments_match_args_c, 7},
    {NULL, NULL, 0}
};

RcppExport void R_init_cmdline_arguments(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
