


#ifndef cmdline_raw_argument_container
#define cmdline_raw_argument_container
#include <string>
#include <vector>
#include <Rcpp.h>
#include "utils/converters/converters.h"
#include "utils/traits/traits.h"
#include "utils/ArgumentList.h"

using Rcpp::wrap, Rcpp::as, Rcpp::List,
      Rcpp::CharacterVector, Rcpp::Shield;

using std::string, std::vector, std::list;

using cmd_args::traits::input_types,
      cmd_args::traits::input_types::individual_input_type,
      cmd_args::traits::input_types::vector_input_type,
      cmd_args::traits::input_types::list_input_type,
      cmd_args::utils::convert_type,
      cmd_args::utils::ArgumentList;

// Simple wrapper for containing a set of raw arguments
// Handles storage and returning argument correctly to parse_func.
namespace cmd_args{
  namespace parser{
    namespace argument{
      class raw_argument{
        public:
          // input_type, name, arguments, outputName
          raw_argument(input_types, string, vector<string>, Rcpp::String);
          raw_argument(string, string, vector<string>, Rcpp::String);
          vector<string> operator()() const;
          void operator+=(string rhs);
          void operator+=(vector<string> rhs);
          void add(string rhs);
          void add(vector<string> rhs);
          // Return depends on the
          ArgumentList digest() const;
          operator ArgumentList() const;
          operator SEXP() const;
        private:
          const input_types type;
          const string name;
          vector<string> data;
          const Rcpp::String outputName; // R string, to allow for R_NilValue
      };
    } // ~argument
  } // ~parser
} // ~cmd_args

#endif
