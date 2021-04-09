
/* Test for option definitions
 *
 * Note we only need to test one option definition, as they are currently all
 * identical (as of 2021-04-08).
 */
// [[Rcpp::plugins("cpp2a")]]

// [[Rcpp::depends(cmdline.arguments)]]

#include <cmd_args/utils/options.h>

#include <string>
// [[Rcpp::export]]
inline int test_option(const SEXP& x, const std::string& msg){
  cmd_args::utils::multi_flag_handling flagOption(x, msg);
  return flagOption;
}

// [[Rcpp::export]]
inline Rcpp::XPtr<cmd_args::utils::multi_flag_handling>
    test_option_xptr(const SEXP& x, const std::string& msg){
  cmd_args::utils::multi_flag_handling flagOption(x, msg);
  Rcpp::XPtr<cmd_args::utils::multi_flag_handling> xPtr(&flagOption);
  return xPtr;
}

