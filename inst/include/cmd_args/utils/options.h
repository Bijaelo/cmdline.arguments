
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
     *
     *
     * Example usage below the definition and in tinytest/cpp/options.cpp
     */
    template<typename T, T V>
    class Option{
    private:
      static const std::unordered_map<char, T> mapper;
      T option = V;

      const string errmsg{"Undefined option provided [Given=%s, valid={%s}]"}; // Needed message. Should never be

      // For error message printing, we need to extract mapper values.
      std::string Options() const {
        std::string res;
        for(auto& [key, value] : mapper){
          res += key;
          res += ", ";
        }
        return res.substr(0, res.size() - 2);
      }

      template<typename t>
      const void throwerr(const t& v){
        Rcpp::stop(errmsg.c_str(),
                   v,
                   Options());
      }
    public:
      /* Constructor overload
       *
       * @details This overload provides a method for creating an option with both an Option in addition to a custom
       * error message.
       *
       * @option string& _option Option
       * @param string _errmsg error message used for printing when inappropriate options are provided. " [Given=%s, valid={%s}]" is added to the
       * argument
       */
      Option(const string& _option, const string _errmsg) : errmsg(_errmsg + " [Given=%s, valid={%s}]"){
        add(_option);
      }

      /* Constructor overload
       *
       * @details This overload provides a method for creating an option with both an Option passed directly from R in addition to a custom
       * error message.
       *
       * @option SEXP& _option A single string SEXP of pryr::sexp_type(_option) == STRSXP.
       * @param string _errmsg error message used for printing when inappropriate options are provided. " [Given=%s, valid={%s}]" is added to the
       * argument
       */
      Option(const SEXP& _option, const string _errmsg) : errmsg(_errmsg + " [Given=%s, valid={%s}]"){
        add(_option);
      }
      /* Constructor overload for providing custom error message
       *
       * @param _errmsg error message used for printing when inappropriate options are provided. " [Given=%s, valid={%s}]" is added to the
       * argument
       */
      Option(const string _errmsg) : errmsg(_errmsg + " [Given=%s, valid={%s}]"){}
      /* Default constructor
       */
      Option(){}

      inline void add(const string& _option){
        if(_option.empty() == true)
          throwerr("empty string");
        char tf = static_cast<char>(std::tolower(static_cast<unsigned char>(_option.front())));
        auto v = mapper.find(tf);
        if(v != mapper.end()){
          this -> option = v -> second;
          return;
        }
        throwerr(option);
      }
      inline void add(const SEXP& _option){
        if(TYPEOF(_option) != STRSXP)
          throwerr(Rf_type2char(TYPEOF(_option)));
        if(Rf_xlength(_option) != 1)
          throwerr("CharacterVector");
        if(_option == NA_STRING)
          throwerr("empty string");
        const char* optionValue = CHAR(STRING_ELT(_option, 0));
        if(strlen(optionValue) == 0)
          throwerr("empty string");
        char tf = static_cast<char>(std::tolower(static_cast<unsigned char>(optionValue[0])));
        auto v = mapper.find(tf);
        if(v != mapper.end()){
          this -> option = v -> second;
          return;
        }
        throwerr(tf);
      }

      inline void add(T _option){
        this -> option = _option;
      }


      inline Option& operator=(const T& _option){
        add(_option);
        return *this;
      }

      inline Option& operator=(const string& _option){
        add(_option);
        return *this;
      }

      inline Option& operator=(const SEXP& _option){
        add(_option);
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


    /* name ideas for "listOption" (which has no intuitive meaning)
     *
     * It is used for the raw arguments obtained from commandArgs(TRUE)
     * It describes whether multible flags should be observed as individual
     * vectors, a combined vector or all individual. This is a parser-global
     * option
     *
     * candidates:
     * - flag_option?
     * - flag_parse_option
     * - argument_option
     * - argument_flag_combining_option
     * - ingestion_option
     * - multi_flag_handling
     * - multi_flag_handling_option
     * - multi_flag_option
     */
    typedef enum{
      individual = 0,
      vectors = 1,
      combine = 2
    } multi_flag_handling_options;

    typedef Option<multi_flag_handling_options, combine> multi_flag_handling;

    template<>
    const std::unordered_map<char, multi_flag_handling_options> multi_flag_handling::mapper = {
      {'i', individual},
      {'v', vectors},
      {'c', combine}
    };

    /* Name ideas for "input_types"
     *
     * While better than "listOption" this still doesn't give an intuitive idea
     * about what it is used for
     *
     * It is used as an option for how arguments should be passed from the
     * argument_container to parser_fun.
     *
     * Candidates:
     * - parser_fun_option
     */
    typedef enum {
      individual_input_type = 0,
      vector_input_type = 1,
      list_input_type = 2
    } parser_fun_options;

    typedef Option<parser_fun_options, vector_input_type> parser_fun_option;

    template<>
    const std::unordered_map<char, parser_fun_options> parser_fun_option::mapper = {
      {'i', individual_input_type},
      {'v', vector_input_type},
      {'l', list_input_type}
    };
  } // ~utils
} // ~cmd_args

#endif
