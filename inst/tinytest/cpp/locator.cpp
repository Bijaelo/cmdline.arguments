
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
list<vector<string>> test_get(XPtr<cmd_args::parser::argument_locator> xPtr,
                              const string& key){
  return (*xPtr).get(key);
}

// [[Rcpp::export]]
bool test_contains(XPtr<cmd_args::parser::argument_locator> xPtr,
                              const string& key){
  return (*xPtr).contains(key);
}

// [[Rcpp::export]]
vector<bool> test_pop(XPtr<cmd_args::parser::argument_locator> xPtr,
                              const string& key){
  vector<bool> res;
  res.push_back((*xPtr).contains(key));
  auto x = (*xPtr).pop(key);
  res.push_back((*xPtr).contains(key));
  return res;
}


// [[Rcpp::export]]
List test_iterate(XPtr<cmd_args::parser::argument_locator> xPtr){
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


