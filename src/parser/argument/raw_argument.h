


#ifndef cmdline_raw_argument_container
#define cmdline_raw_argument_container
#include <string>
#include <vector>
#include <Rcpp.h>
#include "utils/converters/converters.h"
#include "utils/traits/traits.h"
using Rcpp::wrap, Rcpp::as, Rcpp::List, Rcpp::CharacterVector, Rcpp::Shield;
using std::string, std::vector, std::list;
using cmdline_arguments::traits::input_types,
      cmdline_arguments::traits::input_types::individual_input_type,
      cmdline_arguments::traits::input_types::vector_input_type,
      cmdline_arguments::traits::input_types::list_input_type,
      cmdline_arguments::utils::convert_type;

// Simple wrapper for containing a set of raw arguments
// Handles storage and returning argument correctly to parse_func.
namespace cmdline_arguments::parser::argument{
  class raw_argument{
  public:
    raw_argument(input_types, string, vector<string>);
    raw_argument(string, string, vector<string>);
    vector<string> operator()() const;
    void operator+=(string rhs);
    void operator+=(vector<string> rhs);
    void add(string rhs);
    void add(vector<string> rhs);
    // Return depends on the
    Shield<SEXP> digest() const;

  private:
    const input_types type;
    const string name;
    vector<string> data;
  };
}

#endif
/*
cmdline_arguments::parser::argument::raw_argument::raw_argument(
 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >,
 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >,
 std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >,
   std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >)

 */
