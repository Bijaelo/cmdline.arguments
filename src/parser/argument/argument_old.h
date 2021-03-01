
#ifndef _parser_argument_h
#define _parser_argument_h
#include "utils/cmd_coerce.cpp"
#include <algorithm>
#include <cctype>
#include <Rcpp/protection/Shield.h>
#include <regex>
#include "utils/Rapi.h"

enum narg_operator {
  le,
  lq,
  eq,
  gq,
  ge
};


using std::string,
  std::vector,
  Rcpp::stop,
  Rcpp::wrap,
  Rcpp::RObject,
  Rcpp::Rcout,
  Rcpp::warning;

enum actions {
  store,
  store_const,
  store_true,
  store_false,
  append,
  append_const,
  count,
  help // can only be stored once.
};
namespace cmdline_arguments::parser::arguments{

class argument{
      private:
        static int verbose;
        int set_count = 0;
        static const std::map<const string, const actions> action_map;
        static const string& check_flags();
        const string name;
        string narg;
        const actions action;
        actions parseAction(const string&, const bool&, const SEXP&);
        const vector<string> flags;
        bool flag, required;
        const SEXP parse_fun, parse_fun_additional_args, const_, default_;
        vector <string> unparsed_args;
        // parsed_args is taken from cmd_do_docall, and must be protected
        // RObject protects its SEXP without warnings by using black magic
        RObject parsed_args;
        // parse_fun and parse_fun_additional_args are given from R and are protected by nature.
        bool args_parsed = false, args_given = false;
        // This should be fine as well.
        std::tuple<R_xlen_t, narg_operator> narg_parsed;
        void parseNarg(),
          initializeUnparsedArgs(),
          checkArgConflicts(),
          importVerbosity(),
          initializer(),
          setVerbose(const int);
        static const vector<string>& checkFlags(const vector<string>&,
                                                const string&);

      public:
        // Definitions
        argument(const vector<string>&,
                 const string&,
                 const string&,
                 const string&,
                 const bool,
                 const bool,
                 SEXP,
                 SEXP,
                 SEXP,
                 SEXP);
        argument(const string&, // sarg
                 const string&, // larg
                 const string&, // name
                 const string&, // narg
                 const string&, // action
                 const bool, // flag
                 const bool, // required
                 SEXP, // parse_fun
                 SEXP, // parse_fun_args
                 SEXP, // const_
                 SEXP // default_
                 );
        // Only one flag

        // sflag, lflag, name, narg, flag, required
        argument(const string&, // arg
                 const string&, // name
                 const string&, // narg
                 const string&, // action
                 const bool, //flag
                 const bool, // required
                 SEXP, // parse_fun
                 SEXP, // parse_fun_args
                 SEXP, //const_
                 SEXP // default_
                   );



        ~argument(){
          // Used during testing of classes. Is only destroyed after garbage collector is run.
#ifdef _DEBUG_FLAG_
          Rcpp::Rcout << "I have deleted an argument" << std::endl;
#endif
        }

        /* Functions for placing unparsed arguments in the class,
         * activating the parser and extracting the parsed args.
         *
         */
         void setUnparsedArgs(const vector<string>),
           demandReady(),
           parseArgs();
         SEXP getParseFun(),
          getnargparsed(),
          getflags(),
          getsflag(),
          getlflag(),
          getname(),
          getnarg(),
          isflag(),
          isrequired(),
          getUnparsedArgs(),
          getParsedArgs(),
          checkParseFun(SEXP);

        // Maybe parse should return the arguments? Maybe not?

      };
}

#endif
