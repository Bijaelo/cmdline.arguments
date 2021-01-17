
#ifndef _parser_argument_h
#define _parser_argument_h

#include <Rcpp.h>
#include <Rcpp/protection/Shield.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <utils/cmd_coerce.h>

enum narg_operator {
  le,
  lq,
  eq,
  gq,
  ge
};

using std::string, std::vector, Rcpp::stop, Rcpp::wrap;
using cmdline_arguments::utils::coerce::cmd_do_docall;

namespace cmdline_arguments::parser::arguments{


class argument{
      private:
        const string name, narg;
        const vector<string> flags;
        vector <string> unparsed_args;
         SEXP parsed_args, parse_fun, additional_args;
        bool args_parsed = false, args_given = false;
        // This should be fine as well.
        const bool flag, required;
        std::tuple<R_xlen_t, narg_operator> narg_parsed;
        void parse_narg();
        void initializeUnparsedArgs();
      public:
        // Definitions
        // flags, name,
        // This seems to make sense
        argument(const string&,
                 const string&,
                 const string&,
                 const string&,
                 const bool,
                 const bool);
        // Only one flag
        argument(const string&,
                 const string&,
                 const string&,
                 const bool,
                 const bool);



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
        inline void setUnparsedArgs(const vector<string>);
        //SEXP parseArgs();
        inline SEXP getParsedArgs();
        inline void setParseFun(SEXP);
        inline SEXP getParseFun(),
          getnargparsed(),
          getflags(),
          getsflag(),
          getlflag(),
          getname(),
          getnarg(),
          isflag(),
          isrequired();

        inline void demandReady();

        // Maybe parse should return the arguments? Maybe not?

      };
}

#endif
