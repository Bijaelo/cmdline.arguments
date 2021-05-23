// [[Rcpp::plugins("cpp2a")]]

// [[Rcpp::depends(cmdline.arguments)]]
#include <Rcpp.h>
#include <string>
#include <string_view>
#include <cmd_args/parser/argument/container.h>

using namespace cmd_args::parser::argument;
// [[Rcpp::export]]
XPtr<argument_container> test_container_initializer(const SEXP& allow_unknown_flags,
                                                    const SEXP& allow_duplicate_flags){
  return XPtr<argument_container>(new argument_container(Rcpp::as<bool>(allow_unknown_flags), Rcpp::as<bool>(allow_duplicate_flags)));
}

// [[Rcpp::export]]
XPtr<argument_container> test_container_default_initializer(){
  return XPtr<argument_container>(new argument_container());
}


// [[Rcpp::export]]
void test_add_argument(XPtr<argument_container> Xptr,
                       const SEXP& name,
                       const SEXP& flags,
                       const SEXP& meta,
                       const SEXP& action,
                       const SEXP& option,
                       const SEXP& choices){
  Xptr -> add_argument(name,
                       flags,
                       meta,
                       action,
                       option,
                       choices);
}

// [[Rcpp::export]]
R_xlen_t test_narguments(XPtr<argument_container> Xptr){
  return Xptr -> narguments();
}

// [[Rcpp::export]]
R_xlen_t test_nflags(XPtr<argument_container> Xptr){
  return Xptr -> nflags();
}

// [[Rcpp::export]]
list<string> test_getflags(XPtr<argument_container> Xptr){
  list<string> out;
  auto flags = Xptr -> getflags();
  auto e = flags.end();
  for(auto b = flags.begin(); b != e; b++)
    out.push_back(string(*b));
  return out;
}


// [[Rcpp::export]]
void test_printflags(XPtr<argument_container> Xptr){
  auto flags = Xptr -> getflags();
  auto e = flags.end();
  for(auto b = flags.begin(); b != e; b++)
    Rcpp::Rcout << string(*b) << " ";
  Rcpp::Rcout << '\n';
}
