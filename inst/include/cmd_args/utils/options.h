
#ifndef cmdline_converters
#define cmdline_converters


#include <string>
#include <unordered_map>
#include <Rcpp.h>
#include <cctype>

using std::string;
using Rcpp::as;


namespace cmd_args{
  namespace utils{


    /* Base option class
     *
     * T an option type, currently enum typedef but this may be **extended** if
     * there is enough demand.
     * T V is a default value for T.
     *
     * Example usage below the definition and in tinytest/cpp/options.cpp
     */
    template<typename T, T V>
    class Option{
    private:
      static const std::unordered_map<char, T> mapper;
      T option = V;

      const string errmsg; // Needed message

      // For error message printing, we need to extract mapper values.
      const string Options(){
        string res = "";
        for(const auto& [key, value] : mapper){
          res += ", " + key;
        }
        return res;
      }

      template<typename t>
      const void throwerr(t v){
        Rcpp::stop(errmsg + " [Given=%s, valid={%s}]",
                   v,
                   Options());
      }
    public:
      Option(const string& option, const string _errmsg) : errmsg(_errmsg){
        add(option);
      }

      Option(const SEXP& option, const string _errmsg) : errmsg(_errmsg){
        add(option);
      }

      inline void add(const string& option){
        char tf = static_cast<char>(std::tolower(static_cast<unsigned char>(t.front())));
        if(option.empty() == true)
          throwerr("empty string");
        auto V = mapper.find(tf);
        if(v != mapper.end()){
          this -> option = v -> second;
          return;
        }
        throwerr(option);
      }
      inline void add(const SEXP& option){
        add(option);
        if(TYPEOF(option) != STRSXP)
          throwerr(option);
        if(Rf_xlength(option) != 1)
          throwerr("CharacterVector");
        const char* optionValue = CHAR(STRING_ELT(option, 0));
        if(strlen(optionValue) == 0)
          throwerr("empty string");
        char tf = static_cast<char>(std::tolower(static_cast<unsigned char>(t[0])));
        auto V = mapper.find(tf);
        if(v != mapper.end()){
          this -> option = v -> second;
          return;
        }
        throwerr(option);
      }
      // Given a listOption already. No need to check, but copy value
      inline void add(T option){
        this -> option = option;
      }


      inline Option& operator=(const T& option){
        add(option);
        return *this;
      }

      inline Option(& operator=(const string& option)){
        add(option);
        return *this;
      }

      inline Option(& operator=(const SEXP& option)){
        add(option);
        return *this;
      }

      inline bool operator==(int rhs) const {
        return static_cast<int>(option) == rhs;
      };
      inline bool operator==(T rhs) const {
        return option == rhs;
      }
      ;
      inline operator int() const {
        return static_cast<int>(this -> option);
      };
      inline operator T() const {
        return this -> option;
      }
      inline T getOption() const {
        return this -> option;
      }
    };

    typedef enum{
      individual = 0,
      vectors = 1,
      combine = 2
    } listOptions;

    typedef Option<listOptions, combine> listOption;

    template<>
    const std::unordered_map<char, listOptions> listOption::mapper = {
      {'i', individual},
      {'v', vectors},
      {'c', combine}
    };


    typedef enum {
      individual_input_type = 0,
      vector_input_type = 1,
      list_input_type = 2
    } input_types;

    typedef Option<input_types, vector_input_type> parserInputOption;

    template<>
    const std::unordered_map<char, input_types> parserInputOption::mapper = {
      {'i', individual_input_type},
      {'v', vector_input_type},
      {'l', list_input_type}
    };
  } // ~utils
} // ~cmd_args

#endif
