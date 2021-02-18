

#include "parser/narg/narg.h"

namespace cmdline_arguments::parser{
  // Stolen from stackoverflow. (but added s.size())
  inline bool has_only_digits(const string& s){
    return s.size() > 0 && s.find_first_not_of( "0123456789" ) == string::npos;
  }

  // Comparison functions
  inline bool narg::le(R_xlen_t& more) const{
    if(this -> current_value + more >= critical_value){
      // Error message kept (for future extensions). Currently not used anywhere.
      throwerr();
      stop("[arg: %s] accepts <%i argumens, but more were provided.",
           this -> name,
           this -> critical_value);
    }
    return true;
  }
  inline bool narg::leq(R_xlen_t& more) const{
  if(this -> current_value + more > critical_value){
      throwerr();
    }
    return true;
  }
  inline bool narg::ge(R_xlen_t& more) const{
    // We should no get killed for "too few" arguments.
    if(this -> current_value + more <= critical_value){
      return false;
    }
    return true;
  }
  inline bool narg::geq(R_xlen_t& more) const{
    //  We should not get killed for "too few" arguments.
    if(this -> current_value + more < critical_value){
      return false;
    }
    return true;
  }
  inline bool narg::eq(R_xlen_t& more) const{
    if(this -> current_value + more > critical_value){
      throwerr();
    }
    return true;
  }
  // Errors:
  inline void narg::throwerr() const{
    stop((this -> errormsg).c_str(),
         this -> name,
         this -> critical_value);
    return;
  }

  /* Parser function for narg_raw
   * Sets copare_fun, errormsg and critical_value
   */
  inline void narg::parse_narg(){
    if(this -> narg_raw == "?"){
      // 0 or 1 arguments
      this -> critical_value = 1;
      this -> compare_fun = &narg::leq;
      this -> errormsg = "[arg: %s] accepts <=%i arguments, but more were provided.";
    }else if(this -> narg_raw == "+"){
      // 1 or more arguments
      this -> critical_value = 1;
      this -> compare_fun = &narg::geq;
      this -> errormsg = "[arg: %s] accepts >=%i arguments, but less were provided.";
    }else if(this -> narg_raw == "*"){
      // 0 or more arguments (error simply not plausible)
      this -> critical_value = 0;
      this -> compare_fun = &narg::geq;
      this -> errormsg = "[arg: %s] accepts >=%i arguments, but less were provided.";
    }else if(this -> narg_raw == "-"){
      // exactly 0 arguments
      this -> critical_value = 0;
      this -> compare_fun = &narg::eq;
      this -> errormsg = "[arg: %s] accepts exactly %i arguments, but more were provided.";
    }else if(has_only_digits(this -> narg_raw)){
      // Exactly N arguments
      this -> critical_value = (R_xlen_t)std::stoi(this -> narg_raw);
      this -> compare_fun = &narg::eq;
      this -> errormsg = "[arg: %s] accepts exactly %i arguments, but more were provided.";
    }else{
      // Complex number of arguments (N+, N-)
      const char& lc = (this -> narg_raw).back();
      const string rets = (this -> narg_raw).substr(0, (this -> narg_raw).size() - 1);
      if(!has_only_digits(rets))
        stop("[arg: %s] narg should be one of (%s) where N is a positive integer.",
             this -> name,
             "\"?\", \"-\", \"+\", \"*\", \"N\", \"N-\", \"N+\"");
      if(lc == '-'){
        this -> compare_fun = &narg::leq;
        this -> errormsg = "[arg: %s] accepts <=%i arguments, but more were provided.";
      }
      else if(lc == '+'){
        this -> compare_fun = &narg::geq;
        this -> errormsg = "[arg: %s] accepts >=%i arguments, but less were provided.";
      }
      else
        stop("[arg: %s] narg should be one of (%s) where N is a positive integer.",
             this -> name,
             "\"?\", \"+\", \"*\", \"N\", \"N-\", \"N+\"");
      this -> critical_value = (R_xlen_t)stoi(rets);
    }
    return;
  }
  // Initializers
  narg::narg(const string narg_raw_, const string& name_) :
   narg_raw(narg_raw_),
   name(name_){
    this -> parse_narg();
  }
  // user-faced interface
  bool narg::digest(R_xlen_t& more) const {
    // Use store comparison function (eq, le, lq, ge, gq) and test for too many/few arguments.
    // Return true/false, or throw an error in certain cases.
    return std::invoke(compare_fun, this, more);
  }
  // Close up narg
  // test that we satisfy our condition otherwise throw errrormsg
  void narg::close() const{
    // i has to be assigned, because digest takes a reference, so no temp values allowed
    R_xlen_t i = 0;
    // digest may throw an error, or for geq it returns "false" so we have to
    // throw manually
    if(!this -> digest(i)){
      stop("[arg: %s] did not receive sufficient arguments to satisfy condition \"%s\"",
           this -> name,
           this -> narg_raw);
    }
    return;
  }

  void narg::add(R_xlen_t& more){
    this -> digest(more);
    this -> current_value += more;
    return;
  }
  // Operators (only exposed on the c++ side)
  bool narg::operator()(R_xlen_t& rhs) const {
    return this -> digest(rhs);
  };
  bool narg::operator()() const{
    R_xlen_t i = 0;
    return this -> digest(i);
  }
  void narg::operator+=(R_xlen_t& rhs){
    this -> add(rhs);
    return;
  }
  void narg::operator++(){
    R_xlen_t i = 1;
    this -> add(i);
  }
  // Getters
  string narg::getnarg(){
    return this -> narg_raw;
  }
  string narg::geterr(){
    return this -> errormsg;
  }
  string narg::getname(){
    return this -> name;
  }
  R_xlen_t narg::getv(){
    return this -> current_value;
  }
  R_xlen_t narg::getc(){
    return this -> critical_value;
  }

}











