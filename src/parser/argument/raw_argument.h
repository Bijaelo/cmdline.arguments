


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

using cmdline_arguments::traits::input_types,
      cmdline_arguments::traits::input_types::individual_input_type,
      cmdline_arguments::traits::input_types::vector_input_type,
      cmdline_arguments::traits::input_types::list_input_type,
      cmdline_arguments::utils::convert_type,
      cmdline_arguments::utils::ArgumentList;

// Simple wrapper for containing a set of raw arguments
// Handles storage and returning argument correctly to parse_func.
namespace cmdline_arguments::parser::argument{
  class raw_argument{
  public:
    raw_argument(input_types, string, vector<string>, Rcpp::String);
    raw_argument(string, string, vector<string>, Rcpp::String);
    vector<string> operator()() const;
    void operator+=(string rhs);
    void operator+=(vector<string> rhs);
    void add(string rhs);
    void add(vector<string> rhs);
    // Return depends on the
    ArgumentList digest() const;

  private:
    const input_types type;
    const string name;
    vector<string> data;
    const Rcpp::String outputName; // R string, to allow for R_NilValue
  };
}

#endif
