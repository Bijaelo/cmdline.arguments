

//#include <arguments.h> // currently non-existent
//#include <parser.h> // currently non-existent
#include <Rcpp.h>
#include <R.h>
#include <string>
#include "parser/narg/narg.h"
#include "utils/converters/converters.h"
#include "utils/type_name/type_name.h"

using Rcpp::XPtr,
  Rcpp::as,
  Rcpp::wrap;
using std::string;
using cmdline_arguments::parser::narg,
  cmdline_arguments::utils::convert_type;
extern "C" {

  // using section

  SEXP narg_create(SEXP narg_, SEXP name_){
    BEGIN_RCPP
    string narg__ = as<string>(narg_),
      name__ = Rcpp::as<string>(name_);
    XPtr<narg> obj(new narg(narg__, name__));
    return obj;
    END_RCPP
  }
  SEXP narg_add(SEXP ptr, SEXP more){
    BEGIN_RCPP
    XPtr<narg> obj(ptr);
    R_xlen_t m = as<R_xlen_t>(more);
    obj->add(m);
    return R_NilValue;
    END_RCPP
  }
  SEXP narg_close(SEXP ptr){
    BEGIN_RCPP
    XPtr<narg> obj(ptr);
    obj -> close();
    return R_NilValue;
    END_RCPP
  }
  SEXP narg_getnarg(SEXP ptr){
    BEGIN_RCPP
    XPtr<narg> obj(ptr);
    return wrap(obj -> getnarg());
    END_RCPP
  }
  SEXP narg_getv(SEXP ptr){
    BEGIN_RCPP
    XPtr<narg> obj(ptr);
    return wrap(obj -> getv());
    END_RCPP
  }

  SEXP narg_getc(SEXP ptr){
    BEGIN_RCPP
    XPtr<narg> obj(ptr);
    return wrap(obj -> getc());
    END_RCPP
  }
  SEXP narg_geterr(SEXP ptr){
    BEGIN_RCPP
    XPtr<narg> obj(ptr);
    return wrap(obj -> geterr());
    END_RCPP
  }
  SEXP narg_getname(SEXP ptr){
    BEGIN_RCPP
    XPtr<narg> obj(ptr);
    return wrap(obj -> getname());
    END_RCPP
  }
  SEXP _convert(SEXP x){
    BEGIN_RCPP
    R_xlen_t z = convert_type<SEXP>(x);
    return wrap(z);
    END_RCPP
  }
}


#include <R_ext/Rdynload.h>

// borrowed from Matrix
#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}
#define EXTDEF(name)  {#name, (DL_FUNC) &name, -1}

// Create a list of pointers to be exported to R
static const R_CallMethodDef CallEntries[] = {

  // <utils/matching.h>
  {"narg_create", (DL_FUNC) &narg_create, 2},
  {"narg_add", (DL_FUNC) &narg_add, 2},
  {"narg_close", (DL_FUNC) &narg_close, 1},
  {"narg_getnarg", (DL_FUNC) &narg_getnarg, 1},
  {"narg_getv", (DL_FUNC) &narg_getv, 1},
  {"narg_getc", (DL_FUNC) &narg_getc, 1},
  {"narg_geterr", (DL_FUNC) &narg_geterr, 1},
  {"narg_getname", (DL_FUNC) &narg_getname, 1},
  {"convert", (DL_FUNC) &_convert, 1},
  {NULL, NULL, 0}
};

// Register the pointers as Dynamic Symbols for R.
extern "C" void R_init_cmdline_arguments(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, (Rboolean)FALSE);
}
