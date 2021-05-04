

// [[Rcpp::plugins("cpp2a")]]

// [[Rcpp::depends(cmdline.arguments)]]

#include <Rcpp.h>
#include <unordered_set>
#include <string>
#include <string_view>
#include <list>
#include <cmd_args/parser/argument/argument.h>

using namespace cmd_args::parser::argument;
using namespace std;
struct container{
  list<cmd_argument> args;
  unordered_set<string_view> flags;


};
