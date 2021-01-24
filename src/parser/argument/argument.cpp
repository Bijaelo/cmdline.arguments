
#include "parser/argument/argument.h"


/*
 * TODO:
 * handle difference between "store_value", "append" and "help".
 * "help" still needs a print method, which is not implemented yet
 *
 * The question really is "how" should this be handled.
 *
 *
 */
using std::string, std::vector;
using cmdline_arguments::utils::coerce::cmd_do_docall;
using cmdline_arguments::utils::Rapi::cmd_ISNULL;
// Class file for containing individual arguments
namespace cmdline_arguments::parser::arguments{

  // Stolen from stackoverflow.
  bool has_only_digits(const string& s){
    return s.find_first_not_of( "0123456789" ) == string::npos;
  }
  // Initializers
  // argument::argument(const string& _sflag,
  //                    const string& _lflag,
  //                    const string& _name,
  //                    const bool _action) :
  //                    name(_name),
  //                    narg("?"),
  //                    flags({_sflag, _lflag}),
  //                    /*
  //                     * do we actually need "flag" as a value? Dont think so,
  //                     * I think we can just derive whether we're a flag from
  //                     * the remaining info? Oh well, lets just keep it.
  //                     */
  //                    flag(true),
  //                    required(false),
  //                    action(_action),
  //                    _const(R_NilValue),
  //                    help(""),
  //
  //                    {
  //
  // }
  int argument::verbose = -1;
  const std::map<const string, const actions>
    argument::action_map = {
      {"store", store},
      {"store_value", store},
      {"store_const", store_const},
      {"store_true", store_true},
      {"store_false", store_false},
      {"append", append},
      {"append_value", append},
      {"extend", append},
      {"append_const", append_const},
      {"count", count},
      {"help", help}};
  void argument::importVerbosity(){
    if(this -> verbose < 0){
      // This is only executed first time through as verbose is static (shared).
      // Check whether an option is set for verbosity. Default to large value (full verbosity)
      Rcpp::Environment base("package:base");
      Rcpp::Function getOption = base["getOption"];
      SEXP v = getOption("cmdline.arguments.argument.verbosity");
      if(cmd_ISNULL(v)){
        setVerbose(100);;
      }else{
        switch(TYPEOF(v)){
        case REALSXP:
          setVerbose((int)REAL(v)[0]);
          break;
          case INTSXP:
            setVerbose(INTEGER(v)[0]);
            break;
          default:
            setVerbose(100);
          break;
        }

      }
    }
  }
  void argument::initializer(){
    importVerbosity();
    parseNarg();
    initializeUnparsedArgs();
  }
  argument::argument(const vector<string>& args,
                     const string& _name,
                     const string& _narg,
                     const string& _action,
                     const bool _flag,
                     const bool _required,
                     SEXP _parse_fun,
                     SEXP _parse_fun_additional_args,
                     SEXP _const,
                     SEXP _default):
    name(_name),
    narg(_narg),
    action(parseAction(_action, _flag, _const)),
    flags(checkFlags(args, _name)),
    flag(_flag),
    required(_required),
    parse_fun(checkParseFun(_parse_fun)),
    parse_fun_additional_args(_parse_fun_additional_args),
    const_(_const),
    default_(_default)
  {
    initializer();
  }
  argument::argument(const string& _sflag,
                     const string& _lflag,
                     const string& _name,
                     const string& _narg,
                     const string& _action,
                     const bool _flag,
                     const bool _required,
                     SEXP _parse_fun,
                     SEXP _parse_fun_additional_args,
                     SEXP const__,
                     SEXP default__) :
                     name(_name),
                     narg(_narg),
                     action(parseAction(_action, _flag, const__)),
                     flags(checkFlags({_sflag, _lflag}, _name)),
                     flag(_flag),
                     required(_required),
                     parse_fun(checkParseFun(_parse_fun)),
                     parse_fun_additional_args(_parse_fun_additional_args),
                     const_(const__),
                     default_(default__){
    initializer();
  }
  argument::argument(const string& _arg_flag,
                     const string& _name,
                     const string& _narg,
                     const string& _action,
                     const bool _flag,
                     const bool _required,
                     SEXP _parse_fun,
                     SEXP _parse_fun_additional_args,
                     SEXP const__,
                     SEXP default__):
                     name(_name),
                     narg(_narg),
                     action(parseAction(_action, _flag, const__)),
                     flags(checkFlags({_arg_flag}, _name)),
                     flag(_flag),
                     required(_required),
                     parse_fun(checkParseFun(_parse_fun)),
                     parse_fun_additional_args(_parse_fun_additional_args),
                     const_(const__),
                     default_(default__)
                     {
    initializer();
  }


  // Getters:
  // I dont know why the first two are throwing errors. They are not defined elsewhere.
  SEXP argument::getflags(){
    return Rcpp::wrap(this->flags);
  }
  SEXP argument::getsflag(){
    return Rcpp::wrap(this-> flags.front());
  }
  SEXP argument::getlflag(){
    return Rcpp::wrap(this -> flags.back());
  }
  SEXP argument::getname(){
    return Rcpp::wrap(this->name);
  }
  SEXP argument::getnarg(){
    return Rcpp::wrap(this->narg);
  }
  SEXP argument::getnargparsed(){
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
  SEXP argument::getParseFun(){
    return this -> parse_fun;
  }
  SEXP argument::getUnparsedArgs(){
    // FIX ME: This should be able to return either a vector or a list.
    // Needs an option for this.
    return Rcpp::wrap(this -> unparsed_args);
  }
// I honestly couldn't tell you why this does what it does...
// I cant even reproduce this error! but we know that it is stored correctly..
  SEXP argument::isflag(){
    SEXP o = PROTECT(Rf_allocVector(LGLSXP, 1));
    INTEGER(o)[0] = this -> flag ? 1 : 0;
    UNPROTECT(1);
    return o;
  }
  SEXP argument::isrequired(){
    SEXP o = PROTECT(Rf_allocVector(LGLSXP, 1));
    INTEGER(o)[0] = this -> required ? 1 : 0;
    UNPROTECT(1);
    return o;
  }
  // set up some minimum space for our unparsed arguments.
  // nothing fancy.
  void argument::initializeUnparsedArgs(){
    this -> unparsed_args.reserve(std::get<0>(this -> narg_parsed));
  }

  // parsers
  // parseAction exists only for the error message.
  actions argument::parseAction(const string& action,
                                const bool& flag,
                                const SEXP& const_){
    auto match = action_map.find(action);
    if(match == action_map.end())
      stop("action must be one of %s [arg: %s]",
           "\"store\", \"store_value\", \"store_const\", \"store_true\", \"store_false\", \"append\", \"append_value\", \"extend\", \"append_const\", \"count\" or \"help\"",
           this -> name);
    auto match_action = match -> second;
    // default for flag is to store a true value
    if(flag && (match_action != store_true || match_action != store_false))
      return store_true;
    // if const is provided, but store_const is not the action, throw an error (probably should've been set as default)
    if(match_action != store_const && match_action != append_const && !cmd_ISNULL(const_))
      stop("[arg: %s] const was provided but action was not \"store_const\" or \"append_const\"", this -> name);
    return match_action;
  }
  // parseNarg creates a tuple operator for simpler comparison.
  void argument::parseNarg(){
    // Helper lambda: checks for N+ and N-.
    auto f = [&](string a){
      // check to see if the last character is actually + or -
      char& lc = a.back();
      if(lc != '-' && lc != '+')
        stop("%s should be one of (%s) where N is a positive integer [arg: %s]", "nargs", "\"?\", \"+\", \"*\", \"N\", \"N-\", \"N+\"", this -> name);
      narg_operator retc;
      retc = lc == '+' ? gq : lq;
      // Check that everything prior is actually digits.
      std::string rets = a.substr(0, a.size() - 1); // carrier of digits
      if(!has_only_digits(rets))
        stop("%s should be one of (%s) where N is a positive integer [arg: %s]", "nargs", "\"?\", \"+\", \"*\", \"N\", \"N-\", \"N+\"", this -> name);
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
  void argument::setVerbose(const int val){
    this -> verbose = val;
    return;
  }
  // Passing of unparsed and parsing of arguments.
  void argument::setUnparsedArgs(const vector<string> up_args){
    if(this -> flag){
      if(!this -> args_given){
        this -> args_given = true;
      }else
        stop("[arg: %s] flag argument provided multiple times", this -> name);
      return;
    }
    const R_xlen_t n = up_args.size();
    // Handle constant arguments
    if(this -> action == store_const ||
       this -> action == append_const ||
       this -> action == count){
      if(n > 0)
        stop("[arg: %s] const argument received a non-zero length argument",
             this -> name);
      this -> set_count = this -> set_count + 1;
      if(this -> set_count > 1 && this -> action == store_const)
        stop("[arg: %s]: multiple arguments set for store_const argument (did you mean to set action = \"append_const\"?)",
        this -> name);
      this -> args_given = true;
      this -> args_parsed = false;
      return;
    }
    if(n == 0){
      if(this -> action == append_const ||
         (this -> action == store_const && !this -> args_given)){
        // FIXME: const and non-const
      }else
        stop("[arg: %s]: null-length arguments provided for non flag or const argument",
             this -> name);
    }
    const R_xlen_t n_cur = this -> args_given ? this -> unparsed_args.size()  : 0;
    const R_xlen_t& nargs = std::get<0>(this -> narg_parsed);
    const narg_operator& nargs_op = std::get<1>(this -> narg_parsed);
    // Note: The switch only checks for errors, so comparisons are inverted
    switch(nargs_op){
      case le:
        // less than
        if(n + n_cur >= nargs)
          Rcpp::stop("[arg: %s] accepts <%d arguments, while more were provided",
                     this -> name,
                     nargs);
        else
          this -> args_given = true;
        break;
      case lq:
        if(n + n_cur > nargs)
          Rcpp::stop("[arg: %s] accepts <=%d arguments, while more were provided",
                     this -> name,
                     nargs);
        else
          this -> args_given = true;
        break;
      case eq:
        if(n + n_cur > nargs)
          Rcpp::stop("[arg: %s] accepts exactly %d arguments, while more were provided",
                     this -> name,
                     nargs);
        else if (n + n_cur == nargs)
          this -> args_given = true;
        break;
      default:
        // For ">=" and ">" comparisons we can only check once all arguments have been provided.
        break;
    };
    this -> unparsed_args.insert(unparsed_args.end(),
                                 up_args.begin(),
                                 up_args.end());
    this -> args_given = true;
    this -> args_parsed = false;
  }
  // should not be allowed.
  SEXP argument::checkParseFun(SEXP _parse_fun){
    // Some further testing could be useful here.
    if(!Rf_isFunction(_parse_fun) && !Rf_isString(_parse_fun))
      Rcpp::stop("parser.func must be either a function or string");
    return _parse_fun;
  }
  const vector<string>& argument::checkFlags(const vector<string>& flags,
                                             const string& name){
    if(flags.size() > 2)
      stop("[arg: %s] number of flags is greater than 2",
           name);
    // Regex: Contains spaces,
    // starts with a number,
    // should also check for symbols that are not allowed.
    static const std::regex illegal_symbol("\\s|^\\d",
                                           std::regex_constants::ECMAScript);
    for(auto i : flags){
      const string& fi = i;
      if(std::regex_search(fi, illegal_symbol)){
        auto msg = "[arg: %s] flags/names contained whitespace or started with special symbols.";
        stop(msg, name);
      }
    }
    // boost version. Only workable in C++11, while we depend on C++17 or newer
    // static const boost::regex illegal_symbols("(^(\\D?+))|(\\s)");
    // for(auto i : flags){
    //   const string& fi = i;
    //   if(boost::regex_match(fi, illegal_symbols)){
    //     auto msg = "[arg: %s] flags/names contained whitespace or started with special symbols."
    //     "\nWhitespace is not allowed and symbols can only be given at parser level.";
    //     stop(msg, name);
    //   }
    // }
    return flags;
  };
  // Test whether any arguments passed to the class conflicts
  void argument::checkArgConflicts(){
    // CONTINUE HERE ADD VERBOSITY
    if(this -> flag){
      if(this -> required){
        warning("[arg: %s] is marked as both flag and required, setting required = false",
                this -> name);
        this -> required = false;
      }
      if(this -> action != 2 || this -> action != 3)
        stop("[arg: %s] flag arguments must have an action of either \"store_true\" or \"store_false\"");
      // probably need more checks. For example, const and default.

    }else if (this -> required){
      if(std::get<0>(this -> narg_parsed) == 0){
        warning("[arg: %s] argument set as required, but narg indicates 0 is allowed. Setting");
        this -> required = false;
      }

    }
  }
  // cmd_do_docall is having conversion trouble.
  void argument::parseArgs(){
    // If we have already parsed the arguments
    if(this -> args_parsed){
      return;
    }
    // handle flags.
    if(this -> flag){
      if(this -> action == store_true)
        this -> parsed_args = wrap(true);
      else
        this -> parsed_args = wrap(false);
      return;
    }
    // handle store_count
    if(this -> action == count){
      this -> parsed_args = Rcpp::as<Rcpp::RObject>(wrap(this -> set_count));
      this -> args_parsed = true;
    }
    // If we don't have any arguments, simply return NULL.
    if(! (this -> args_given) )
      return;
    // Handle store_const. Note if store_const == 0, then args_given == false
    if(this -> action == store_const || this -> action == append_const){
      Rcpp::List out(this -> set_count);
      for(int i = 0; i < this -> set_count; i++){
        out[i] = this -> const_;
      }
      this -> parsed_args = Rcpp::as<Rcpp::RObject>(out);
      this -> args_parsed = true;
      return;
    }

    // add unparsed_args to parse_fun_additional_args as a named element called "args"
    R_xlen_t n = Rf_length(this -> parse_fun_additional_args) + 1, i = 0;
    SEXP a_args_names = PROTECT(Rf_getAttrib(this -> parse_fun_additional_args,
                                             R_NamesSymbol));
    Rcpp::List args(n);
    Rcpp::CharacterVector names(n);
    args[i] = this -> getUnparsedArgs();
    for(i = 1; i < n; i++){
      args[i] = VECTOR_ELT(this -> parse_fun_additional_args, i);
      names[i] = STRING_ELT(a_args_names, i);
    }
    args.attr("names") = names;
    UNPROTECT(1);
    Rcpp::Environment env = Rcpp::new_env(29);
    this -> parsed_args =
      Rcpp::as<Rcpp::RObject>(cmd_do_docall(this -> parse_fun,
                                            Rcpp::wrap(args),
                                            env,
                                            this -> name));
    // for static evaluation within class instance.
    this -> args_parsed = true;
  }

// FIXME!
// Needs to handle const values (maybe?).
  SEXP argument::getParsedArgs(){
    if(!this -> args_given){
      // note that flags default to their action
      // (eg: store_true -> true, store_false -> false)
      // All other arguments default to "NULL". parsers skip over R_NilValue and NILSXP
      return this -> default_;
    }
    if(!this -> args_parsed)
      stop("[arg: %s] getParsedArgs called prior to parseargs", this -> name);
    return this -> parsed_args;
  }
  void argument::demandReady(){
    if(!this -> flag && !this -> args_given){
      if(this -> required){
        // Checking default is not necessary, as required should mean default is NULL
        // But I might want to change that behaviour later on.
        if(cmd_ISNULL(this -> default_))
          stop("[arg: %s] required argument was not provided with any arguments", this -> name);
      }
    }
  }
}
