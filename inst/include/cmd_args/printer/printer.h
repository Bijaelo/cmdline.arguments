
/* Printer class
 *
 * Used for printing help messages
 * Wraps around parserFunction, but allows users to specify a custom function or
 * string handling arguments.
 * Still a work in progress (Performs zero formatting of inputs)
 *
 */


#ifndef cmd_args_printer_header
#define cmd_args_printer_header
#include <string>
#include <map>

#if !defined(_WIN32) && !defined(_WIN64)
# include <regex>
#endif

#include "parser/pfunc/pfunc.h"
using std::string, std::map;
using Rcpp::Function, Rcpp::CharacterVector, Rcpp::List;
namespace cmd_args{
  namespace printer{
    // Printer is definitely not finished.
    // We need static members for values such as text width (for each individual
    // column) and line-breaks. But for now we'll just make it a simple "print
    // text without formatting width." Later we'll look at more complex
    // stuff. It is not my forté (None of this is).
    class printer{

      string msg, frontEnclose, backEnclose;
      const bool internal;
      cmd_args::parser::parserFunction printFun;
    public:
      inline void print(map<string, string> values){
        if(internal){
          // We are told to use internal functions,
          for(const auto& [key, value] : values){
            // I would prefer to use regex, but it seems to fail on windows.
            // So we need to fall back to standard line-search replacement.
#if defined(_WIN32) || defined(_WIN64)
            std::string find = frontEnclose + key + backEnclose;
            std::size_t pos = msg.find(find, 0);
            while(pos != std::string::npos){
              msg.replace(pos, find.size(), value);
              pos = msg.find(find, pos + value.size());
            };
#else
            std::regex reg(frontEnclose + key + backEnclose);
            msg = std::regex_replace(msg, reg, value);
#endif
          }
          printFun(msg);
        }else{
          // User provided own function. We've got to convert our map to a list,
          // and pass each value to the function as parameter
          values["frontEnclose"] = frontEnclose;
          values["backEnclose"] = backEnclose;
          R_xlen_t n = values.size();
          CharacterVector vals(n), names(n);
          CharacterVector::iterator vi = vals.begin(), ni = names.begin();
          for(const auto& [key, value] : values){
            *vi = value;
            *ni = key;
            vi++; ni++;
          }
          vals.attr("names") = names;
          printFun(List(vals));
        }
      }
      printer(string _msg, string _frontEnclose, string _backEnclose) :
        msg(_msg), frontEnclose(_frontEnclose), backEnclose(_backEnclose),
        internal(true), printFun(Function("cat")){}
      printer(Function _msg) : internal(false), printFun(_msg){}


    };
  } // ~printer
} // ~cmd_args

#endif
