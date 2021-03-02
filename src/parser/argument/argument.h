


// Argument class. Container for holding one or more rawArgs, narg and

#ifndef cmdline_arguments_argument_h
#define cmdline_arguments_argument_h

#include <Rcpp.h>
#include <list>
#include <vector>
#include <string>
#include <set> // for storing unique flags.
#include <iterator>
#include "parser/narg/narg.h"
#include "parser/argument/raw_argument.h"
#include "parser/pfunc/pfunc.h"
#include "printer/printer.h" // error in wrap here. Not certain why that is at the moment
using namespace cmdline_arguments::parser;
using std::string, std::list, std::set, std::greater;
using Rcpp::Function, Rcpp::RObject, Rcpp::stop, Rcpp::List, Rcpp::CharacterVector;
//using cmdline_arguments::printer::printer;

namespace cmdline_arguments::parser::argument{
  class Argument {
   const narg Narg;
    //
    parserFunction pfunc;


    const string name, action, nargRaw; // action should likely not just be a string ?

    const set<string> flags;
    list<raw_argument> rawArgs;

    // cant be stored in SEXP due to the garbage collector.
    list<RObject> parsedArgs;

    //help is either string or Function. We'll check upon initialization.
    const List pArgs;
    const RObject help; // <== We've got to think this one through.

   void testHelp();
  public:
    // constructors take copied values not addresses, as this might be the "first" time we store these values.

    // flags, name, action, narg, help, parserFunction, parserArgs
    template<typename T>
    Argument(vector<string>, string, string, string, SEXP, T, List);
    template<typename T>
    Argument(string, string, string, string, SEXP, T, List);
  };
}


#endif
