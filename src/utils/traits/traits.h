

#ifndef cmdline_traits
#define cmdline_traits
// Traits
// Abuse the definition used in Rcpp
#include <Rcpp/traits/integral_constant.h>
using std::string, std::vector;
using Rcpp::List, Rcpp::CharacterVector, Rcpp::wrap;
namespace cmdline_arguments::traits{
  // This works fine, but it would be very sensible to have a mapper
  // from string input_types usable at compile time.
  // Eg: option allows for "individual", "vector" and "list", but
  typedef enum {
    individual_input_type = 0,
    vector_input_type = 1,
    list_input_type = 2
  } input_types;

  /* Base template (extended from integral_constant).
   *
   * Only aware that it has "data". This is a partial definition, "parse"
   * is specialized outside the body.
   *
   * Usages:
   * [definition name] x(data);
   * auto y = x.parse();
   * x += more_data;
   * x.parse();
   *
   * [definition name] x;
   * x = data;
   * auto y = x.parse();
   *
   * Still needs testing:
   * input_type::parse()
   * input_type::operator()
   * input_type::operator(ini)
   * input_type::operator+=)
   * input_type::operator==
   */
  template <typename _T, _T _V, typename _TD, typename _D>
  struct input_type{
    static const _T               value = _V;
    typedef _T                    value_type;
    // _D is not necessary?
    typedef input_type<_T, _V, _TD, _D> type;
    input_type operator=(_TD& rhs){
      input_type x(rhs);
      return x;
    };
    void operator+=(_TD& rhs){
      data += rhs;
    };
    _TD& operator()(){
      return data;
    };
    void operator()(_TD& ini){
      data = ini;
    };
    _D parse() const;
  private:
    // parse should only be called **once**.
    // And the parse result (after going through parse_func) has no home in
    // input_types. Instead it will be contained outside.
    _TD data;
  };
  // Specialization of "parse". The remaining functions are fully specified.
  template<>
  inline List input_type<input_types, individual_input_type, vector<string>, List>::parse() const{
    // abuse that wrap turns our vector<string> into a list of individual components.
    return wrap(data);
  };
  template<>
  inline CharacterVector input_type<input_types, vector_input_type, vector<string>, CharacterVector>::parse() const{
    CharacterVector x = wrap(data);
    return x;
  };
  template<>
  inline List input_type<input_types, list_input_type, vector<string>, List>::parse() const{
    // This might not be necessary, but I want to be darn sure that I get a CharacterVector.
    CharacterVector x = wrap(data);
    List y(x);
    return y;
  };

// individual_type
  //typedef Rcpp::traits::integral_constant<input_types, individual_input_type>
  typedef input_type<input_types, individual_input_type, vector<string>, List>
    individual_input;
// vector_type
  //typedef Rcpp::traits::integral_constant<input_types, vector_input_type>
  typedef input_type<input_types, vector_input_type, vector<string>, CharacterVector>
    vector_input;
// list_type
  //typedef Rcpp::traits::integral_constant<input_types, list_input_type>
  typedef input_type<input_types, list_input_type, vector<string>, List>
    list_input;
  /*
  // typedef generators.
  template<typename _T, _T _V, typename _Z>
  _Z generate_input_type(_T _V);

  template<>
  individual_input generate_input_type<input_types, individual_input_type, individual_input>(input_types){
    individual_input x;
    return x;
  };

  template<>
  list_input generate_input_type<input_types, list_input_type, list_input>(input_types){
    list_input x;
    return x;
  };

  template<>
  vector_input generate_input_type<input_types, vector_input_type, vector_input>(input_types){
    vector_input x;
    return x;
  };
   */
}

#endif


