


#include "utils/converters/converters.h"

namespace cmdline_arguments::utils{

  static const std::map<char, input_types> type_mapper = {
    {'i', individual_input_type},
    {'v', vector_input_type},
    {'l', list_input_type}
  };

template<typename T>
input_types convert_type(T t){
  const char* fmt = "Unknown input type, unable to convert to string: [type=%s].";
  //Rcpp::stop(fmt, "test");
  Rcpp::stop(fmt, type_name<decltype(t)>());
  return individual_input_type;
};
template<typename T>
input_types convert_type(T t, string name){
  const char* fmt = "[arg: %s] Unknown input type, unable to convert to string: [type=%s].";
  //Rcpp::stop(fmt, name, "test");
  Rcpp::stop(fmt, name, type_name<decltype(t)>());
  return individual_input_type;
};


  // Simple conversion: Test if the string is contain in our mapper,
  // if it is, we return the equivalent enum member
  // I may change this later, so that it can take an argument name as input.
  template<>
  input_types convert_type<string>(string t){
    // Safely cast to lower-case char
    // Following the note on cppreference of std::tolower
    char tf = static_cast<char>(std::tolower(static_cast<unsigned char>(t.front())));

    auto v = type_mapper.find(tf);
    if(v != type_mapper.end()){
     return v -> second;
    }
    Rcpp::stop("Undefined input type provided [Given=%s, valid={%s}].",
               t,
               "individual[_type], vector[_type], list[_type]");
  };
  template<>
  input_types convert_type<string>(string t, string name){
    // Safely cast to lower-case char
    // Following the note on cppreference of std::tolower
    char tf = static_cast<char>(std::tolower(static_cast<unsigned char>(t.front())));
    auto v = type_mapper.find(tf);
    if(v != type_mapper.end()){
      return v -> second;
    }
    Rcpp::stop("[arg: %s], Undefined input type provided [Given=%s, valid={%s}].",
               name,
               t,
               "individual[_type], vector[_type], list[_type]");
  };
  // this one is not being caught correctly. Interesting. Or maybe it is not
  template<>
  input_types convert_type(SEXP t){
    // Maybe test with Rf_isString first and
    // throw an exception that is better related to this function specifically?
    return convert_type(as<string>(t));
  };
  template<>
  input_types convert_type(SEXP t, string name){
    return convert_type(as<string>(t), name);
  }

}
/*
 cmdline_arguments::traits::input_types
 cmdline_arguments::utils::convert_type
   <std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >
 (std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >,
  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&)
 */
