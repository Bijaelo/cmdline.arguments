

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
      cmdline_arguments::traits::listOptions,
      cmdline_arguments::traits::listOptions::individual,
      cmdline_arguments::traits::listOptions::vectors,
      cmdline_arguments::traits::listOptions::combine,
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



  // Much better option for converting type.
  // I'll keep the alternative as it is already implemented.
  static const std::map<char, listOptions> listOptionsMapper = {
    {'i', individual},
    {'v', vectors},
    {'c', combine}
  };

  class listOption{
  private:
    listOptions option = combine; // Defaut to combine.
  public:
    listOption(string t){
      add(t);
    };
    listOption(listOptions t){
      add(t);
    };
    listOption(){};

    inline void add(string t){
      char tf = static_cast<char>(std::tolower(static_cast<unsigned char>(t.front())));
      auto v = listOptionsMapper.find(tf);
      if(v != listOptionsMapper.end()){
        option = v -> second;
        return;
      }
      Rcpp::stop("Undefined option specified for handling of flags provided multiple times [Given=%s, valid={%s}]",
                 t,
                 "individual, vector, combine");
    }
    inline void add(listOptions t){
      this -> option = t;
    }
    template<typename T>
    inline void operator+=(T t){
      add(t);
    }
    template<typename T>
    inline listOption& operator=(T t){
      add(t);
      return *this;
    }

    inline bool operator==(int rhs) const {
      return option == rhs;
    };
    inline bool operator==(listOptions rhs) const {
      return option == rhs;
    };
    inline operator int() const {
      return static_cast<int>(this -> option);
    };
    inline operator listOptions() const {
      return this -> option;
    }
    inline listOptions getOption() const {
      return this -> option;
    }
  };

}

#endif


/*
 `cmdline_arguments::traits::input_types
 cmdline_arguments::utils::convert_type<std::__cxx11::basic_string<char, std::char_traits<char>,
 std::allocator<char> > >(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)'
 */
