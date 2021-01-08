
#ifndef _parser_argument_h
#define _parser_argument_h
#include <Rcpp.h>
#include <string>
#include <vector>
#include <utils/cmd_coerce.h>



using std::string, std::vector, Rcpp::stop, Rcpp::wrap;

namespace cmdline_arguments::parser::arguments{
      class argument{
      private:
        const string name, narg;
        const vector<string> flags;
        vector <string> unparsed_args;
        SEXP parsed_args, parse_fun, additional_args;
        bool args_parsed = false, args_given = false;
        std::tuple<R_xlen_t, char> narg_parsed;
        void parse_narg();
      public:
        // Definitions
        // flags, name,
        argument(const string&,
                 const string&,
                 const string&,
                 const string&);
        // Only one flag
        argument(const string&,
                 const string&,
                 const string&);


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
        void setUnparsedArgs(const vector<string>);
        SEXP parseArgs();
        SEXP getParsedArgs();
        void setParseFun(SEXP);
        SEXP getParseFun();

        // Maybe parse should return the arguments? Maybe not?
        //void parse();
        //Rcpp::String getname(), getsarg(), getlarg(), getnarg(), getdest();
        //Rcpp::CharacterVector getarg;
        SEXP getsarg(), getlarg(), getname(), getnarg();
        //std::string sarg, larg, name, narg, dest;
        //std::vector<std::string> arg;
        //bool flag, required, vectorized;
        //SEXP def, parsedArg;
        //Rcpp::List default_val;
        //Rcpp::Function parse_func;

      };
}

#endif
