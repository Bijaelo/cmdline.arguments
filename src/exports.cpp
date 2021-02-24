

//#include <arguments.h> // currently non-existent
//#include <parser.h> // currently non-existent
#include <Rcpp.h>
#include <R.h>
#include <string>
#include "parser/narg/narg.h"

#include "utils/type_name/type_name.h"
#include "utils/converters/converters.h"
#include "parser/argument/raw_argument.h"
#include "RcppApi/pairlist.h"

#include "parser/pfunc/pfunc.h"

using Rcpp::XPtr,
  Rcpp::as,
  Rcpp::wrap;
using std::string;
using cmdline_arguments::parser::narg,
  cmdline_arguments::utils::convert_type,
  cmdline_arguments::parser::argument::raw_argument;
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

  SEXP _raw_argument(SEXP type, SEXP name, SEXP data, SEXP outputName = R_NilValue){
    BEGIN_RCPP
    string t = as<string>(type), n = as<string>(name);
    Rcpp::String on = as<Rcpp::String>(outputName);
    XPtr<raw_argument> obj(new raw_argument(t, n, as<vector<string>>(data), on));
    return obj;
    END_RCPP
  }
  SEXP _raw_argument_add(SEXP ptr, SEXP data){
    BEGIN_RCPP
    R_xlen_t n = Rf_length(data);
    XPtr<raw_argument> obj(ptr);
    if(n == 1){
      obj -> add(as<string>(data));
    }else if(n > 1){
      obj -> add(as<vector<string>>(data));
    }
    return ptr;
    END_RCPP
  }
  SEXP _raw_argument_digest(SEXP ptr){
    BEGIN_RCPP
    XPtr<raw_argument> obj(ptr);
    return (obj -> digest()).data;
    END_RCPP
  }
  SEXP do_call(SEXP fun, SEXP args){
    BEGIN_RCPP
    Rcpp::Function f(fun);
    Rcpp::List a = Rcpp::as<List>(args);
    cmdline_arguments::utils::ArgumentList aa(a);
    return f(aa);
    END_RCPP
  }
  SEXP do_call2(SEXP fun, SEXP args){
    BEGIN_RCPP
    Rcpp::Function f(fun);
    Rcpp::List a = Rcpp::as<List>(args);
    cmdline_arguments::utils::ArgumentList aa(a);
    return aa.data;
    END_RCPP
  }
  SEXP execute_pfunc(SEXP fun, SEXP args1, SEXP args2){
    BEGIN_RCPP
    Rcpp::Function f(fun);
    Rcpp::List a = as<List>(args1), b = as<List>(args2);
    cmdline_arguments::parser::parserFunction pf(fun, a);
    return pf(b);
    END_RCPP
  }

  /*
  SEXP make_pfunc(SEXP fun, SEXP args, SEXP name){
    BEGIN_RCPP
    using namespace cmdline_arguments::parser;
    XPtr<parserFunction> ptr(new parserFunction(Rcpp::as<Rcpp::Function>(fun), Rcpp::as<string>(name), Rcpp::as<Rcpp::List>(args)));
    return ptr;
    END_RCPP
  }
  SEXP make_pfunc_argless(SEXP fun, SEXP name){
    BEGIN_RCPP
    using namespace cmdline_arguments::parser;
    XPtr<parserFunction> ptr(new parserFunction(Rcpp::as<Rcpp::Function>(fun), Rcpp::as<string>(name)));
    return ptr;
    END_RCPP
  }
  // Need to execute with more args
  SEXP exec_pfunc(SEXP pfunc, SEXP args){
    BEGIN_RCPP
    using namespace cmdline_arguments::parser;
    XPtr<parserFunction> ptr(pfunc);
    return ptr -> operator()(Rcpp::as<List>(args));
    END_RCPP
  }*/
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

  {"raw_argument_create", (DL_FUNC) &_raw_argument, 3},
  {"raw_argument_add", (DL_FUNC) &_raw_argument_add, 2},
  {"raw_argument_digest", (DL_FUNC) &_raw_argument_digest, 1},
  {"do_call", (DL_FUNC) &do_call, 2},
  {"do_call2", (DL_FUNC) &do_call2, 2},
  {"execute_pfunc", (DL_FUNC) &execute_pfunc, 3},
  // {"do_call2", (DL_FUNC) &do_call2, 2},
  // {"do_call3", (DL_FUNC) &do_call3, 2},
  //{"make_pfunc", (DL_FUNC) &make_pfunc, 3},
  // {"make_pfunc_argless", (DL_FUNC) &make_pfunc_argless, 2},
  // {"exec_pfunc", (DL_FUNC) &exec_pfunc, 2},
  {NULL, NULL, 0}
};

// Register the pointers as Dynamic Symbols for R.
extern "C" void R_init_cmdline_arguments(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, (Rboolean)FALSE);
}
/*
 undefined reference to
 cmdline_arguments::parser::parserFunction::parserFunction
 <Rcpp::Function_Impl
  <Rcpp::PreserveStorage>, std::__cxx11::basic_string<char, std::char_traits<char>,
  std::allocator<char>
 >,
 Rcpp::ArgumentList_Impl<19, Rcpp::PreserveStorage> >(Rcpp::Function_Impl<Rcpp::PreserveStorage>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, Rcpp::ArgumentList_Impl<19, Rcpp::PreserveStorage>)
 */
