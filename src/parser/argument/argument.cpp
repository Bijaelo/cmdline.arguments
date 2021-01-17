
#include "parser/argument/argument.h"
// Check argument.h for all "using" statements etc.

using std::string, std::vector;

// Class file for containing individual arguments
namespace cmdline_arguments::parser::arguments{

  bool has_only_digits(const string& s){
    return s.find_first_not_of( "0123456789" ) == string::npos;
  }
  // Initializers
  argument::argument(const string& _sflag,
                     const string& _lflag,
                     const string& _name,
                     const string& _narg,
                     const bool _flag,
                     const bool _required) :
                     name(_name),
                     narg(_narg),
                     flags({_sflag, _lflag}),
                     flag(_flag),
                     required(_required){
    parse_narg();
    initializeUnparsedArgs();
  }
  argument::argument(const string& _arg_flag,
                     const string& _name,
                     const string& _narg,
                     const bool _flag,
                     const bool _required) :
                     name(_name),
                     narg(_narg),
                     flags({_arg_flag}),
                     flag(_flag),
                     required(_required)
                     {
    parse_narg();
    initializeUnparsedArgs();
  }


  // Getters:
  // I dont know why the first two are throwing errors. They are not defined elsewhere.
  inline SEXP argument::getflags(){
    return Rcpp::wrap(this->flags);
  }
  inline SEXP argument::getsflag(){
    return Rcpp::wrap(this-> flags.front());
  }
  inline SEXP argument::getlflag(){
    return Rcpp::wrap(this -> flags.back());
  }
  inline SEXP argument::getname(){
    return Rcpp::wrap(this->name);
  }
  inline SEXP argument::getnarg(){
    return Rcpp::wrap(this->narg);
  }
  inline SEXP argument::getnargparsed(){
    Rcpp::List o(2);
    o[0] = std::get<0>(this -> narg_parsed);
    switch (std::get<1>(this -> narg_parsed)){
      case le:
        o[1] = "<";
        break;
      case lq:
        o[1] = "<=";
        break;
      case eq:
        o[1] = "==";
        break;
      case gq:
        o[1] = ">=";
        break;
      case ge:
        o[1] = ">";
        break;
    }
    return o;
  }
// I honestly couldn't tell you why this does what it does...
// I cant even reproduce this error! but we know that it is stored correctly..
  inline SEXP argument::isflag(){
    SEXP o = PROTECT(Rf_allocVector(LGLSXP, 1));
    INTEGER(o)[0] = this -> flag ? 1 : 0;
    UNPROTECT(1);
    return o;
  }
  inline SEXP argument::isrequired(){
    SEXP o = PROTECT(Rf_allocVector(LGLSXP, 1));
    INTEGER(o)[0] = this -> required ? 1 : 0;
    UNPROTECT(1);
    return o;
  }
  // set up some minimum space for our unparsed arguments.
  // nothing fancy.
  inline void argument::initializeUnparsedArgs(){
    this -> unparsed_args.reserve(std::get<0>(this -> narg_parsed));
  }
  // parsers
  inline void argument::parse_narg(){
    // Helper lambda: checks for N+ and N-.
    auto f = [&](string a){
      // check to see if the last character is actually + or -
      char& lc = a.back();
      if(lc != '-' && lc != '+')
        stop("%s should be one of (%s) where N is a positive integer", "nargs", "\"?\", \"+\", \"*\", \"N\", \"N-\", \"N+\"");
      narg_operator retc;
      retc = lc == '+' ? gq : lq;
      // Check that everything prior is actually digits.
      std::string rets = a.substr(0, a.size() - 1); // carrier of digits
      if(!has_only_digits(rets))
        stop("%s should be one of (%s) where N is a positive integer", "nargs", "\"?\", \"+\", \"*\", \"N\", \"N-\", \"N+\"");
      std::tuple<R_xlen_t, narg_operator> out = {(R_xlen_t)std::stoi(rets), retc};
      return out;
    };
    if(this -> narg == "?")
      this -> narg_parsed = std::make_tuple<R_xlen_t, narg_operator>(1, lq);
    else if(this -> narg == "+")
      this -> narg_parsed = std::make_tuple<R_xlen_t, narg_operator>(1, gq);
    else if(this -> narg == "*")
      this -> narg_parsed = std::make_tuple<R_xlen_t, narg_operator>(0, gq);
    // Special scenario. 0 args only (Flag)
    else if(this -> narg == "-")
      this -> narg_parsed = std::make_tuple<R_xlen_t, narg_operator>(0, eq);
    // exactly N args:
    else if(has_only_digits(this -> narg))
      this -> narg_parsed = {(R_xlen_t)std::stoi(this -> narg), eq};
    // at least N args (N+) or at most N args (N-).
    else
      this -> narg_parsed = f(narg);
  }

  // Passing of unparsed and parsing of arguments.
  inline void argument::setUnparsedArgs(const vector<string> up_args){
    if(this -> flag){
      if(!this -> args_given){
        this -> args_given = true;
        Rcpp::
      }else
        stop("flag argument \"%s\" provided multiple times");
      return;
    }
    const R_xlen_t n = up_args.size();
    if(n == 0)
      stop("unexpected error [arg: %s]: null-length arguments provided",
           this -> name);
    const R_xlen_t n_cur = this -> args_given ? this -> unparsed_args.size()  : 0;
    const R_xlen_t& nargs = std::get<0>(this -> narg_parsed);
    const narg_operator& nargs_op = std::get<1>(this -> narg_parsed);
    // Note: The switch only checks for errors, so comparisons are inverted
    switch(nargs_op){
      case le:
        // less than
        if(n + n_cur >= nargs)
          Rcpp::stop("argument \"%s\" accepts <%d arguments, while more were provided",
                     this -> name,
                     nargs);
        else
          this -> args_given = true;
        break;
      case lq:
        if(n + n_cur > nargs)
          Rcpp::stop("argument \"%s\" accepts <=%d arguments, while more were provided",
                     this -> name,
                     nargs);
        else
          this -> args_given = true;
        break;
      case eq:
        if(n + n_cur > nargs)
          Rcpp::stop("argument \"%s\" accepts exactly %d arguments, while more were provided",
                     this -> name,
                     nargs);
        else if (n + n_cur == nargs)
          this -> args_given = true;
        break;
      default:
        // For ">=" and ">" comparisons we can only check once all arguments have been provided.
        break;
    };
    //for(auto i : up_args)
    //  this -> unparsed_args.push_back(i);
    this -> unparsed_args.insert(unparsed_args.end(),
                                 up_args.begin(),
                                 up_args.end());
    Rcout << this -> unparsed_args.size() << std::endl;
  }
  inline void argument::setParseFun(SEXP parse_fun){
    // Some further testing could be useful here.
    if(!Rf_isFunction(parse_fun) && !Rf_isString(parse_fun))
      Rcpp::stop("parser.func must be either a function or string");
    this -> parse_fun = parse_fun;
  }

/* // this function is having trouble with type conversion.
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
    Rcpp::Environment env = Rcpp::new_env(29);
    this -> parsed_args =
      cmd_do_docall(this -> parse_fun,
                    Rcpp::wrap(args),
                    env,
                    this -> name);
    // for static evaluation within class instance.
    this -> args_parsed = true;
    return this -> parsed_args;
    END_RCPP
  }*/

// FIXME!
  SEXP argument::getParsedArgs(){
    if(!this -> args_given)
      stop("The argument has not been provided with any arguments.");
    stop("To be implemented");
    //if(!this -> args_parsed)
    //  this -> parseArgs();
    return (this -> parsed_args);
  }
  void argument::demandReady(){
    if(!this -> args_given && this -> required)
      stop("Required argument \"%s\" did not receive sufficient input",
           this -> name);
  }
}
