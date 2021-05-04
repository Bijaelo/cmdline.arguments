// [[Rcpp::plugins("cpp2a")]]

// [[Rcpp::depends(cmdline.arguments)]]
#include <Rcpp.h>
#include <string>
#include <string_view>
#include <cmd_args/parser/argument/argument.h>

using namespace cmd_args::parser::argument;
// [[Rcpp::export]]
XPtr<cmd_argument> test_argument_initializer(const SEXP& name,
                                             const SEXP& flags,
                                             const SEXP& meta,
                                             const SEXP& action,
                                             const SEXP& choices,
                                             const SEXP& option){
  return XPtr<cmd_argument>(new cmd_argument(name, flags, meta, action, option, choices));
}
