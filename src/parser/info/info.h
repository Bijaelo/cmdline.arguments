

// Container for holding information.

#ifndef cmdline_arguments_argument_info_h
#define cmdline_arguments_argument_info_h

#include <memory>
#include <string>
#include <list>
#include <vector>
#include <map>
using namespace std;
using namespace Rcpp;

#include "parser/narg/narg.h"
#include "parser/argument/raw_container.h"
#include "parser/argument/raw_argument.h"
#include "parser/pfunc/pfunc.h"
#include "printer/printer.h"


namespace cmdline_arguments::parser::arguments{

  /* c-style struct.
   *
   * Simply used to make all parts of the argument available to other parts of
   * the function. There are some "wtf" moments here, but it'll make it possible
   * to do a few things that we wouldn't otherwise.
   * At least not while I know of it.
   *
   *
   *
   */
  typedef struct {

    string name,
      rawNarg,
      rawOption,
      flagOption,
      dest,
      rawAction,
      metaVar;

    vector<string> flags,
      choices;

    RObject parseFun,
      helpIn,
      defaultVal;

    bool required;

    weak_ptr<cmdline_arguments::parser::narg> narg;
    weak_ptr<cmdline_arguments::parser::parserFunction> parserFunction;
    weak_ptr<cmdline_arguments::parser::argument::raw_Container> rawContainer;
    weak_ptr<cmdline_arguments::parser::argument::raw_argument> rawArgument;
    weak_ptr<cmdline_arguments::printer::printer> helpPrinter;

  } argument_info;

}

#endif
