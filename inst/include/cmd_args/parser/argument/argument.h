


// Argument class. Container for holding one or more rawArgs, narg and

#ifndef cmd_args_argument_h
#define cmd_args_argument_h

#include <Rcpp.h>
#include <cmd_args/utils/options.h>
#include <cmd_args/utils/sugar.h>
#include <string>
#include <string_view>
#include <algorithm>
#include <unordered_set>
#include <iterator>
using namespace std;
using namespace cmd_args::utils::sugar;

//using cmd_args::printer::printer;
namespace cmd_args::parser::argument{
  // Container for all the information needed and passed around inside an Argument
  struct Info{
    string name,
    narg, // narg is here, we dont need to have a string in the Narg class at all! Just use a string_view
    meta,
    action,
    option;
    // helpFlags are stored in argument_container not Argument itself.
    string_view helpFlags;

    unordered_set <string> flags, choices;

    RObject parser,
            help,
            default_,
            const_;

    bool positional;

    Info() = default;
    Info(const Info& other) = default;
    Info(Info&& other){
      name = std::move(other.name);
      narg = std::move(other.narg);
      meta = std::move(other.meta);
      action = std::move(other.action);
      option = std::move(other.option);
      helpFlags = std::move(other.helpFlags); // No real effect It'll just copy
      flags = std::move(other.flags);
      choices = std::move(other.choices);
      parser = other.parser;
      help = other.help;
      default_ = other.default_;
      const_ = other.const_;
      positional = std::move(other.positional);
    }
  };
  class Argument{

    Info info;

    // transform string ot uppercase safely.
    inline void str_toupper(const string& x, string& out){
      out.clear();
      transform(x.begin(), x.end(), std::back_inserter(out), [](unsigned char c){ return toupper(c);});
    }
    inline const void check_flagsize(){
      if(info.flags.size() == 0){
        info.positional = true;
      }
    }
    inline const void assignFlags(const SEXP& flags){
      if(flags != R_NilValue && TYPEOF(flags) == STRSXP){
        R_xlen_t n = Rf_xlength(flags);
        if(!ALTREP(flags)){
          SEXP* ptr = STRING_PTR(flags);
          for(R_xlen_t i = 0; i < n; i++, ptr++){
            if(*ptr != NA_STRING){
              string_view  c(CHAR(*ptr));
              if(c.size() != 0){
                info.flags.insert(string(CHAR(*ptr)));
              }
            }
          }
        }else{
          for(R_xlen_t i = 0; i < n; i++){
            const SEXP fi = STRING_ELT(flags, i);
            if(fi != NA_STRING){
              string_view c(CHAR(fi));
              if(c.size() != 0)
                info.flags.insert(string(CHAR(fi)));
            }
          }
        }
        check_flagsize();
      }else
        Rcpp::stop("invalid flags provided for argument");
    }
    inline const void assignFlags(const vector<string>& flags){
      for(const auto& i : flags){
        info.flags.insert(i);
      }
      check_flagsize();
    }
    inline const void assignFlags(const string& flags){
      if(flags.size() == 0)
        Rcpp::stop("Empty string provided for flag name. For positional arguments set flag to NULL.");
      info.flags.insert(flags);
    }
    inline const void assignFlags(const unordered_set<string>& flags){
      info.flags = flags;
      check_flagsize();
    }


    // Extract the longest flag an strip non-alphanumeric characters to use as
    inline const void assignNameFromFlags(){
      if(info.positional)
        Rcpp::stop("Argument must have either a name or flags provided");
      struct{
        bool stop = false;
        bool operator()(unsigned char c){
          if(stop)
            return false;
          if(std::isalnum(c)){
            stop = true;
            return false;
          }else
            return true;
        }
      } remover;
      size_t mx = 0;
      // Pointer to flags,
      string* name;
      for(auto i : info.flags){
        size_t c = i.size();
        if(c > mx){
          mx = c;
          name = &i;
        }
      }
      // Copy name into info and remove predicates.
      info.name = *name;
      info.name.erase(remove_if(info.name.begin(), info.name.end(), remover), info.name.end());
    }
    // FIXME: should use the largest flag name
    inline const void assignName(const string& name){
      if(name.size() == 0){
        assignNameFromFlags();
      }else
        info.name = name;
    }
    inline const void assignName(const SEXP& name){
      if(cmd_args::utils::sugar::is<string>(name)){
        SEXP* xPtr = STRING_PTR(name);
        if(*xPtr == NA_STRING)
          assignNameFromFlags();
        else{
          string strName(CHAR(*xPtr));
          assignName(strName);
        }
      }else if(name == R_NilValue){
        assignNameFromFlags();
      }else
        stop("non-string name specified for argument");
    }
    // Creates a meta string if it is not present already
    inline const void assignMeta(const string& meta){
      if(meta.size() > 0)
        info.meta = meta;
      else
        str_toupper(info.name, info.meta);
    }
    inline const void assignMeta(const SEXP& meta){
      if(meta != R_NilValue && TYPEOF(meta) == STRSXP && Rf_length(meta) == 1){
        assignMeta(as<string>(meta));
      }else
        str_toupper(info.name, info.meta);
    }

    // Action lookup table
    inline static const unordered_set<string> actions{"store", "store_value", "store_true", "store_false", "const", "store_const"};
    // test that a valid action was provided.
    inline void assignAction(const string& action){
      if(action.size() == 0)
        info.action = "store";
      else if(actions.find(action) == actions.end())
        Rcpp::stop("[Arg: %s] action must be one of (%s).",
                   info.name,
                   "\"store[_value]\", \"const\", \"store_const\", \"store_true\", \"store_false\"");
      else
        info.action = action;
    }

    // more cumbersome test for action as SEXP
    inline const void assignAction(const SEXP& action) {
      if(cmd_args::utils::sugar::is<string>(action)){
        SEXP* xPtr = STRING_PTR(action);
        string strAction;
        if(*xPtr == NA_STRING)
          strAction = "store"s;
        else
          strAction = CHAR(*xPtr);
        return assignAction(strAction);
      }else
        Rcpp::stop("[Arg: %s] action must be one of (%s).",
                   info.name,
                   "\"store[_value]\", \"const\", \"store_const\", \"store_true\", \"store_false\"");
    }
    cmd_args::utils::parser_fun_option Option{"Invalid option provided to parser for handling multiple flags"};
    template<typename T>
    inline const void assignOption(const T& option){
      Option.add(option);
    }

    inline const void assignChoices(const vector<string>& choices){
      for(auto i: choices){
        if(i.size() != 0)
          info.choices.insert(i);
      }
    }
    inline const void assignChoices(const SEXP& choices){
      if(choices != R_NilValue){
        R_xlen_t n = Rf_xlength(choices);
        if(n != 0){
          if(TYPEOF(choices) != STRSXP)
            stop("[Arg: %s] Choices must be a character vector", info.name);
          if(n == 1)
            stop("[Arg: %s] Choices must be a character vector of length > 1", info.name);
          if(!ALTREP(choices)){
            SEXP* xPtr = STRING_PTR(choices);
            for(R_xlen_t i = 0; i < n; i++, xPtr++){
              if(*xPtr != NA_STRING){
                string_view c(CHAR(*xPtr));
                if(c.size() != 0)
                  info.choices.insert(string(CHAR(*xPtr)));
              }
            }
          }else{
            for(R_xlen_t i = 0; i < n; i++){
              SEXP ii = STRING_ELT(choices, i);
              if(ii != NA_STRING){
                string_view c(CHAR(ii));
                if(c.size() != 0)
                  info.choices.insert(string(CHAR(ii)));
              }
            }
          }
          if(info.choices.size() <= 1)
            stop("[Arg: %s] Choices must be a character vector of length > 1", info.name);
        }
      }
    }
    // Base initializer. Same for SEXP's and non-SEXP's due to function overloading
    template<typename T, typename O, typename F>
    inline void init(const T& name,
                     const F& flags,
                     const T& meta,
                     const T& action,
                     const O& option,
                     const T& choices){
      assignFlags(flags);
      assignName(name);
      assignMeta(meta);
      assignAction(action);
      assignOption(option);
      assignChoices(choices);
    }
  public:
    /* Default initializer.
     *
     *
     */
    template<typename T, typename O, typename F>
    inline Argument(const T& name,
                    const F& flags,
                    const T& meta,
                    const T& action,
                    const O& option,
                    const T& choices){
      init(name, flags, meta, action, option, choices);
      };

    inline void helpFlags(string_view helpFlags){
      info.helpFlags = helpFlags;
    }
    inline const string_view helpFlags() const {
      return info.helpFlags;
    }
    /* Force parsing to be performed of this argument.
     *
     * Throws an error if the argument is required and insufficient arguments were supplied.
     */
    inline void parse(){

    };

    inline const unordered_set<string>& flags() const {
      return info.flags;
    }
    inline const string_view name() const {
      return info.name;
    }

  };
  // Typedef that exists to avoid collision with Rcpp::Argument. (Great question why that isn't documented or implemented as Argument__impl).
  typedef cmd_args::parser::argument::Argument cmd_argument;
}



#endif
