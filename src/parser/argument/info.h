

// Container for holding information.

#ifndef cmd_args_argument_info_h
#define cmd_args_argument_info_h

#include <string>
#include <vector>
#include <Rcpp.h>
using namespace std;
using namespace Rcpp;


namespace cmd_args::parser::arguments{

  /* c-style struct.
   *
   * Simple container for holding shared information across multiple components.
   *
   * Many components of the argument exist to "encapsulate" a certain part of
   * the cmdline argument reader. Such as "raw_argument", "raw_Container" and
   * "parserFunction". However most of these require some or all of the same
   * arguments,
   */
  typedef struct {

    const string name,
      narg,
      meta,
      action,
      rawPassingOption,
      rawIngestionOpton,
      helpFlags;

    const vector<string> flags,
      choices;

    const RObject parseFun,
      help,
      defaultVal,
      constVal;

    const bool required;

  } argument_info;

}

#endif
