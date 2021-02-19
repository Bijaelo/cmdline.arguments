

#ifndef cmdline_converters
#define cmdline_converters

// Cast from string to cmdline_arguments::traits::input_types

#include <string>
#include <map>
#include <Rcpp.h>
#include <cctype>
#include "utils/type_name/type_name.h"
#include "utils/traits/traits.h"

using std::string;
using Rcpp::as;
using cmdline_arguments::traits::input_types,
      cmdline_arguments::traits::input_types::individual_input_type,
      cmdline_arguments::traits::input_types::vector_input_type,
      cmdline_arguments::traits::input_types::list_input_type,
      cmdline_arguments::utils::type_name;


namespace cmdline_arguments::utils{

  // Standard behaviour: Throw error if we dont know the specific type.

  // this is the thing that's happening. It is throwing this error.
  template<typename T>
  input_types convert_type(T);
  template<typename T>
  input_types convert_type(T, string);
  template<>
  input_types convert_type<SEXP>(SEXP);
  template<>
  input_types convert_type<SEXP>(SEXP, string);
  template<>
  input_types convert_type<string>(string);
  template<>
  input_types convert_type<string>(string, string);




}

#endif


/*
 `cmdline_arguments::traits::input_types
 cmdline_arguments::utils::convert_type<std::__cxx11::basic_string<char, std::char_traits<char>,
 std::allocator<char> > >(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)'
 */
