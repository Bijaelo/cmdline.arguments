
#ifndef _parser_argument_h
#define _parser_argument_h

#include "utils/cmd_coerce.cpp"
#include <algorithm>
#include <cctype>
#include <Rcpp/protection/Shield.h>
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
  Rcpp::Rcout;
//using cmdline_arguments::utils::coerce::cmd_do_docall;

namespace cmdline_arguments::parser::arguments{


class argument{
      private:
        const string name, narg;
        const vector<string> flags;
        vector <string> unparsed_args;
        // parsed_args is taken from cmd_do_docall, and must be protected
        // RObject protects its SEXP without warnings by using black magic
        RObject parsed_args;
        // parse_fun and additional_args are given from R and are protected by nature.
        const SEXP parse_fun, additional_args;
        bool args_parsed = false, args_given = false;
        // This should be fine as well.
        const bool flag, required;
        //const SEXP _const
        std::tuple<R_xlen_t, narg_operator> narg_parsed;
        void parse_narg();
        void initializeUnparsedArgs();
      public:
        // Definitions

        // sflag, lflag, name, narg, flag, required
        // This seems to make sense
        argument(const string&,
                 const string&,
                 const string&,
                 const string&,
                 const bool,
                 const bool,
                 SEXP,
                 SEXP
                 );
        // Only one flag

        // sflag, lflag, name, narg, flag, required
        argument(const string&,
                 const string&,
                 const string&,
                 const bool,
                 const bool,
                 SEXP,
                 SEXP);



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
