
#include "utils/cmd_coerce.h"
namespace cmdline_arguments::utils::coerce {

static int rinternal_position_(){
  int i = -1;
  char* out;
  do{
    i++;
    out = R_FunTab[i].name;
  }
  while(std::strcmp(out, ".Internal") != 0);
  return i;
}

SEXP cmd_do_docall(SEXP what,
                  SEXP args,
                  Rcpp::Environment envir,
                  std::string argname = ""){
  // input checking
  if(!(Rf_isFunction(what) || (Rf_isString(what) && Rf_length(what) == 1))){
    if(argname == "")
      stop("parser.func %s is neither a function nor a character string. 'parser.func' must be a function or character string", argname);
    else
      stop("'parser.func' must be a fnction or character string");
  }
  if (!Rf_isNull(args) && !Rf_isNewList(args))
    stop("Unexpected error while parsing arguments. The arguments given to parser.func were of the wrong type. Please open an issue on the package github!");
  const R_xlen_t& n = Rf_length(args); // safe because args are within scope
  if(n < 1)
    stop("Unexpected error while parsing arguments. The length of arguments given to parser was zero.");
  SEXP names;
  PROTECT(names = Rf_getAttrib(args, R_NamesSymbol));
  /* Generate pairlists to store the call to be made.
   * Note c = call = Rf_allocList means c and call point to the same address
   * and thus as we move through c with CAR and CDR, call will stay pointing to
   * the first position.
   */
  SEXP c, call;
  PROTECT(c = call = Rf_allocList(n + 1));
  SET_TYPEOF(c, LANGSXP);
  if(Rf_isString(what)){
    std::string str = (std::string)Rf_translateChar(STRING_ELT(what, 0));
    if(str == ".Internal") stop("passing .Internal as parser function is not allowed");
    SETCAR(c, Rf_install(str.c_str())); //Rf_install takes a const char* eg. c_str.
  }else{
    // I haven't gotten access to the internal (iteratively updated) FUNTAB.
    // Until I  figure out how to access the active array I am using a hacky-method.
#if defined(USE_RINTERNALS) && defined(CMDLINE_ARGUMENTS_MAYBE_IN_THE_FUTURE)
    char* str = PRIMNAME(what);
    if(TYPEOF(what) == SPECIALSXP){
      if(!strcmp(str, ".Internal")){
        stop("passing \".Internal\" as parser function is not allowed");
      }
    }
#else
    // rinternal_position is only executed once per cpp session and stored.
    // This method ensures stability across R-versions (if .Internal is moved in R_FunTab)
    if(TYPEOF(what) == SPECIALSXP && rinternal_position_() == (what -> u.primsxp.offset))
      stop("passing \".Internal\" as parser function is not allowed.");
#endif
    SETCAR(c, what);
  }
  c = CDR(c);
  for(R_xlen_t i = 0; i < n; i++){
    // Taken from do_docall in coerce. I am not sure if NEW is defined in one
    // of our dependencies, so i'll keep it as is. This might cause unexpected
    // behaviour in the future, and should be kept in check.

    SETCAR(c, VECTOR_ELT(args, i));
    // Inlining of ItemName (marked by attribute_hidden)
    if(names != R_NilValue){
      auto namei = STRING_ELT(names, i);
      if(namei != R_NilValue && CHAR(namei)[0] != '\0')
        SET_TAG(c, Rf_installTrChar(namei));
    }
    // Move to the next element so we
    c = CDR(c);
  }
  UNPROTECT(2);
  return Rf_eval(call, envir);
}
}
