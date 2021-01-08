
#include "parser/argument/argument.h"
#include <algorithm>
#include <cctype>
// Check argument.h for all "using" statements etc.

// Class file for containing individual arguments
namespace cmdline_arguments::parser::arguments{

  bool has_only_digits(const string& s){
    return s.find_first_not_of( "0123456789" ) == string::npos;
  }
  // Initializers
  argument::argument(const string& _sflag,
                     const string& _lflag,
                     const string& _name,
                     const string& _narg) :
                     name(_name),
                     narg(_narg),
                     flags({_sflag, _lflag}){
    parse_narg();
  }
  argument::argument(const string& _flag,
                     const string& _name,
                     const string& _narg) :
                     name(_name),
                     narg(_narg),
                     flags({_flag})
                     {
    parse_narg();
  }
  // Getters:
  SEXP argument::getsarg(){
    return Rcpp::wrap(this->flags[0]);
  }
  SEXP argument::getlarg(){
    return Rcpp::wrap(this -> flags.rbegin());
  }
  SEXP argument::getname(){
    return Rcpp::wrap(this->name);
  }
  SEXP argument::getnarg(){
    return Rcpp::wrap(this->narg);
  }

  void argument::parse_narg(){
    // Helper lambda: checks for N+ and N-.
    auto f = [&](string a){
      // check to see if the last character is actually + or -
      char& lc = a.back();
      if(lc != '-' && lc != '+')
        stop("%s should be one of (%s) where N is a positive integer", "nargs", "\"?\", \"+\", \"*\", \"N\"");
      char retc;
      retc = lc == '+' ? '+' : '-';
      // Check that everything prior is actually digits.
      string rets; // carrier of digits
      rets.reserve(a.size() - 1);
      auto b2 = a.begin(), e = a.end() - 1;
      // manual version of has_only_digits. Maybe slightly faster, as I have to convert it later.
      for(; b2 != e; b2++)
        if(std::isdigit((unsigned char)(*b2)) != 0)
          stop("%s should be one of (%s) where N is a positive integer", "nargs", "\"?\", \"+\", \"*\", \"N\"");
        else
          rets.push_back(*b2);
      std::tuple<R_xlen_t, char> out = {(R_xlen_t)std::stoi(rets), retc};
      return out;
    };
    if(this -> narg == "?")
      this -> narg_parsed = std::make_tuple<R_xlen_t, char>(2, '<');
    else if(this -> narg == "+")
      this -> narg_parsed = std::make_tuple<R_xlen_t, char>(0, '>');
    else if(this -> narg == "*")
      this -> narg_parsed = std::make_tuple<R_xlen_t, char>(-1, '>');
    // Special scenario. 0 args only (Flag)
    else if(this -> narg == "-")
      this -> narg_parsed = std::make_tuple<R_xlen_t, char>(0, '=');
    // exactly N args:
    else if(has_only_digits(this -> narg))
      this -> narg_parsed = {(R_xlen_t)std::stoi(this -> narg), '='};
    // at least N args (N+)
    else
      this -> narg_parsed = f(narg);
  };

  // Passing of unparsed and parsing of arguments.
  void argument::setUnparsedArgs(const vector<string> up_args){
    if(this -> args_given){
      stop("arguments has already been given");
    }

    // Ugly code:
    size_t n = up_args.size();
    if(this -> narg == "?"){
      if(this -> args_given)
        stop("argument takes 0 or 1 argument but received multiple [arg: %s]", this -> name);
      if(n > 1)
        stop("argument takes 0 or 1 argument but received at least %d [arg: %s]", n, this -> name);
    }else if(this -> narg == "+"){
      if(n < 1)
        stop("argument takes at least 1 argument but received 0 [arg: %s]", this -> name);
    }else if (has_only_digits(this -> narg)){
      auto nargint = std::stoi(this -> narg);
      if((int)n != nargint)
        stop("argument takes exactly %d arguments but received %d [arg: %s]", nargint, n, this -> name);
    }else
      stop("%s should be one of (%s) where N is a positive integer", "nargs", "\"?\", \"+\", \"*\", \"N\"");
    this -> unparsed_args = up_args;
  }
  void argument::setParseFun(SEXP parse_fun){
    // Some further testing could be useful here.
    if(!Rf_isFunction(parse_fun) && !Rf_isString(parse_fun))
      Rcpp::stop("parser.func must be either a function or string");
    this -> parse_fun = parse_fun;
  }

  // Needs to be tested. But it is a pretty simple function.
  SEXP argument::parseArgs(){
    // If we have already parsed the arguments
    if(this -> args_parsed)
      return this -> parsed_args;
    // If we don't have any arguments, simply return NULL.
    if(! (this -> args_given) )
      return R_NilValue;
    BEGIN_RCPP
    // add unparsed_args to additional_args as a named element called "args"
    R_xlen_t n = Rf_length(this -> additional_args) + 1, i;
    SEXP a_args_names = PROTECT(Rf_getAttrib(this -> additional_args,
                                             R_NamesSymbol));
    Rcpp::List args(n);
    Rcpp::CharacterVector names(n);
    for(i = 0; i < n - 1; i++){
      args[i] = VECTOR_ELT(this -> additional_args, i);
      names[i] = STRING_ELT(a_args_names, i);
    }
    args[i] = Rcpp::wrap(this -> unparsed_args);
    names[i] = "args";
    args.attr("names") = names;
    UNPROTECT(1);
    this -> parsed_args = cmdline_arguments::utils::coerce::
      cmd_do_docall(this -> parse_fun,
                    Rcpp::wrap(args),
                    Rcpp::new_env(29),
                    this -> name);
    // for static evaluation within class instance.
    this -> args_parsed = true;
    return this -> parsed_args;
    END_RCPP
  }

// FIXME!
  SEXP argument::getParsedArgs(){
    if(!this -> args_given)
      stop("The argument has not been provided with any arguments.");
    if(!this -> args_parsed)
      this -> parseArgs();
    return (this -> parsed_args);
  }
}
