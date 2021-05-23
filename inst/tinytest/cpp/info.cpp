
/* Test for the helper functions "make_info" used in arguments
 *
 * ## Experimental -
 *
 * Basically just for constructing a semi-C-style struct of information to be
 * passed around in arguments.
 * Some fields will be removed/added as time goes o
 */

// [[Rcpp::plugins("cpp2a")]]

// [[Rcpp::depends(cmdline.arguments)]]

#include <Rcpp.h>
#include <cmd_args/parser/argument/info.h>

using namespace Rcpp;
using namespace cmd_args::parser::argument;
// [[Rcpp::export]]
XPtr<argument_info> test_make_info_single(const SEXP& name,
                                          const SEXP& narg,
                                          const SEXP& meta,
                                          const SEXP& action,
                                          const SEXP& rawPassingOption,
                                          const SEXP& rawIngestionOption,
                                          const SEXP& flags,
                                          const SEXP& choices,
                                          const SEXP& parseFun,
                                          const SEXP& help,
                                          const SEXP& defaultVal,
                                          const SEXP& constVal,
                                          const SEXP& required){
  return XPtr<argument_info>(make_info_single(name,
                                              narg,
                                              meta,
                                              action,
                                              rawPassingOption,
                                              rawIngestionOption,
                                              flags,
                                              choices,
                                              parseFun,
                                              help,
                                              defaultVal,
                                              constVal,
                                              required));
}

// [[Rcpp::export]]
List test_make_info_multiple(const SEXP& listInfo){
  List res;
  for(auto i: make_argument_info_from_row_list(listInfo)){
    res.push_back(XPtr<argument_info>(i));
  }
  return res;
}

