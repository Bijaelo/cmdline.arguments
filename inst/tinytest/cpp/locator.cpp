
/* test for argument locator class.
 *
 * Tests consists of initializing, finding arguments, extracting arguments and iterating through arguments
 */


// [[Rcpp::plugins("cpp2a")]]

// [[Rcpp::depends(cmdline.arguments)]]

#include <cmd_args/parser/argument/locator.h>

#include <Rcpp.h>
using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
XPtr<cmd_args::parser::argument_locator> test_initializer(vector<string> args,
                                                          vector<string> lookups){
  XPtr<cmd_args::parser::argument_locator> xPtr(new cmd_args::parser::argument_locator(args, lookups));
  return xPtr;
}

// [[Rcpp::export]]
XPtr<cmd_args::parser::argument_locator> test_default_initializer(){
  XPtr<cmd_args::parser::argument_locator> xPtr(new cmd_args::parser::argument_locator());
  return xPtr;
}


// [[Rcpp::export]]
list<vector<string>> test_get(XPtr<cmd_args::parser::argument_locator> xPtr,
                              const string& key){
  if(!(xPtr -> is_parsed()))
    stop("Argument not parsed, please parse the arguments first");
  return (*xPtr).get(key);
}

// [[Rcpp::export]]
bool test_contains(XPtr<cmd_args::parser::argument_locator> xPtr,
                              const string& key){
  if(!(xPtr -> is_parsed()))
    stop("Argument not parsed, please parse the arguments first");
  return (*xPtr).contains(key);
}

// [[Rcpp::export]]
vector<bool> test_pop(XPtr<cmd_args::parser::argument_locator> xPtr,
                              const string& key){
  if(!(xPtr -> is_parsed()))
    stop("Argument not parsed, please parse the arguments first");
  vector<bool> res;
  res.push_back((*xPtr).contains(key));
  auto x = (*xPtr).pop(key);
  res.push_back((*xPtr).contains(key));
  return res;
}


// [[Rcpp::export]]
List test_iterate(XPtr<cmd_args::parser::argument_locator> xPtr){
  if(!(xPtr -> is_parsed()))
    stop("Argument not parsed, please parse the arguments first");
  List out;
  auto b = (*xPtr).cbegin(), e = (*xPtr).cend();
  for(; b != e; b++){
    List temp;
    temp.push_back(wrap(b -> first));
    temp.push_back(wrap(b -> second));
    out.push_back(temp);
  }
  return out;
}

// [[Rcpp::export]]
void test_insert_lvalue(XPtr<cmd_args::parser::argument_locator> xPtr, SEXP& values){
  vector<string> vs = as<vector<string>>(values);
  xPtr -> insert(vs);
}

// [[Rcpp::export]]
void test_insert_rvalue(XPtr<cmd_args::parser::argument_locator> xPtr, SEXP& values){
  xPtr -> insert(std::move(as<vector<string>>(values)));
}

// [[Rcpp::export]]
void test_parse(XPtr<cmd_args::parser::argument_locator> xPtr, vector<string> lookups){
  xPtr -> parse(lookups);
}
