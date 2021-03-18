

//#include <arguments.h> // currently non-existent
//#include <parser.h> // currently non-existent
#include <Rcpp.h>
#include <R.h>
#include <string>
#include <map>


#include "parser/narg/narg.h"
#include "utils/type_name/type_name.h"
#include "utils/converters/converters.h"
#include "parser/argument/raw/class.h"
#include "parser/argument/raw/container.h"
#include "parser/argument/class.h"
#include "parser/pfunc/pfunc.h"
#include "parser/argument/locator.h"
#include "printer/printer.h"
#include "parser/argument/info.h"

using Rcpp::XPtr,
  Rcpp::as,
  Rcpp::wrap;
using std::string;
using cmd_args::parser::narg,
  cmd_args::utils::convert_type,
  cmd_args::parser::argument::raw_argument,
  cmd_args::parser::argument::raw_Container;
using namespace
  cmd_args::parser::argument;
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
  SEXP _convertList(SEXP x){
    BEGIN_RCPP
    cmd_args::utils::listOption(as<string>(x));
    return wrap(R_NilValue);
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
    cmd_args::utils::ArgumentList aa(a);
    return f(aa);
    END_RCPP
  }
  SEXP do_call2(SEXP fun, SEXP args){
    BEGIN_RCPP
    Rcpp::Function f(fun);
    Rcpp::List a = Rcpp::as<List>(args);
    cmd_args::utils::ArgumentList aa(a);
    return aa.data;
    END_RCPP
  }
  SEXP execute_pfunc(SEXP fun, SEXP args1, SEXP args2){
    BEGIN_RCPP
    Rcpp::Function f(fun);
    Rcpp::List a = as<List>(args1), b = as<List>(args2);
    cmd_args::parser::parserFunction pf(fun, a);
    return pf(b);
    END_RCPP
  }


  SEXP make_pfunc_argless(SEXP fun, SEXP name){
    BEGIN_RCPP
    using namespace cmd_args::parser;
    XPtr<parserFunction> ptr(new parserFunction(Rcpp::as<Rcpp::Function>(fun), Rcpp::as<string>(name)));
    return ptr;
    END_RCPP
  }
  // Need to execute with more args
  SEXP exec_pfunc(SEXP pfunc, SEXP args){
    BEGIN_RCPP
    using namespace cmd_args::parser;
    XPtr<parserFunction> ptr(pfunc);
    return ptr -> operator()(Rcpp::as<List>(args));
    END_RCPP
  }
  SEXP print_something_internal(SEXP values, SEXP msg){
    BEGIN_RCPP
    std::vector<string> vals = as<std::vector<string>>(values);
    std::vector<string> nams = as<std::vector<string>>(Rf_getAttrib(values, R_NamesSymbol));
    std::map<string, string> ma = {};
    for(size_t i = 0; i < vals.size(); i++){
      ma[nams[i]] = vals[i];
    }
    cmd_args::printer::printer pr(as<string>(msg), "", "");
    pr.print(ma);
    END_RCPP
  }
  SEXP print_something_external(SEXP values, SEXP fun){
    BEGIN_RCPP
    std::vector<string> vals = as<std::vector<string>>(values);
    std::vector<string> nams = as<std::vector<string>>(Rf_getAttrib(values, R_NamesSymbol));
    std::map<string, string> ma = {};
    for(size_t i = 0; i < vals.size(); i++){
      ma[nams[i]] = vals[i];
    }
    cmd_args::printer::printer pr(as<Function>(fun));
    pr.print(ma);
    END_RCPP
  }

  SEXP argument_locator_test(SEXP args, SEXP flags){
    BEGIN_RCPP
    vector<string> vargs = as<vector<string>>(args),
      vflags = as<vector<string>>(flags);

    cmd_args::parser::argument_locator loc(vargs, vflags);
    List out;
    out["first"] = loc.pop("-f");
    out["second"] = loc.get("--f");
    out["third"] = loc.get("positionals");
    out["fourth"] = loc.get("--g");
    return wrap(out);

    return wrap(R_NilValue);
    END_RCPP
  }
  // raw_Container tests:
  /* 1) Creation
   * 2) argument addition
   * 3) iteration
   * 4) extract (through iteration)
   */
  SEXP create_raw_container(SEXP raw_option,
                            SEXP name,
                            SEXP outputName,
                            SEXP listOption,
                            SEXP narg){
    BEGIN_RCPP
    // Something here is crashing... the question is what.
    XPtr<raw_Container> xPtr(new raw_Container(as<string>(raw_option),
                                               as<string>(name),
                                               as<string>(outputName),
                                               as<string>(listOption),
                                               as<string>(narg)));

    return wrap(xPtr);

    END_RCPP
  }
  SEXP raw_container_add_vector(SEXP ptr, SEXP _vector){
    BEGIN_RCPP
    XPtr<raw_Container> xPtr(ptr);
    (*xPtr) += as<vector<string>>(_vector);
    return wrap(xPtr);
    END_RCPP
  }
  SEXP raw_container_add_string(SEXP ptr, SEXP _string){
    BEGIN_RCPP
    XPtr<raw_Container> xPtr(ptr);

    xPtr -> add(as<string>(_string));
//    (*xPtr) += as<string>(_string);
    return wrap(xPtr);
    END_RCPP
  }
  SEXP raw_container_extract(SEXP ptr){
    BEGIN_RCPP
    XPtr<raw_Container> xPtr(ptr);
    List out;
    for(const auto i : *xPtr){
      out.push_back(wrap(i)); //no care for efficiency here. Just smash the object in.
    }
    return out;
    END_RCPP
  }

  SEXP make_argument(SEXP name,
                     SEXP narg,
                     SEXP meta,
                     SEXP action,
                     SEXP rawPassingOption,
                     SEXP rawIngestionOption,
                     SEXP helpFlags,
                     SEXP flags,
                     SEXP choices,
                     SEXP parseFun,
                     SEXP help,
                     SEXP defaultVal,
                     SEXP constVal,
                     SEXP required){
    BEGIN_RCPP
    argument_info info = make_info_single(name,
                                          narg,
                                          meta,
                                          action,
                                          rawPassingOption,
                                          rawIngestionOption,
                                          helpFlags,
                                          flags,
                                          choices,
                                          parseFun,
                                          help,
                                          defaultVal,
                                          constVal,
                                          required);
    XPtr<argument_info> ptr(new argument_info(info));
    return wrap(ptr);
    END_RCPP
  }

  SEXP make_argument_multiple(SEXP listInfo){
    BEGIN_RCPP
    vector<argument_info> info = make_argument_info_from_row_list(listInfo);

    XPtr<vector<argument_info>> ptr(new vector<argument_info>());
    for(auto i : info){
      ptr->push_back(i);
    }
    return wrap(ptr);
    END_RCPP
  }

  SEXP check_argument_single(SEXP Xptr){
    BEGIN_RCPP
    XPtr<argument_info> ptr(Xptr);
    Rcout << ptr -> name << "\n";
    return Xptr;
    END_RCPP
  }

  SEXP check_argument_multiple(SEXP Xptr){
    BEGIN_RCPP
    XPtr<vector<argument_info>> ptr(Xptr);
    R_xlen_t n = ptr -> size();
    for(R_xlen_t i = 0; i < n; i++){
      Rcout << ((*ptr)[i]).name << "\n";
    }
    return Xptr;
    END_RCPP
  }

  SEXP create_info(SEXP name,
                   SEXP narg,
                   SEXP meta,
                   SEXP action,
                   SEXP rawPassingOption,
                   SEXP rawIngestionOption,
                   SEXP helpFlags,
                   SEXP flags,
                   SEXP choices,
                   SEXP parseFun,
                   SEXP help,
                   SEXP defaultVal,
                   SEXP constVal,
                   SEXP required){
    BEGIN_RCPP
    cmd_args::parser::argument::make_info_single(name,
                                                 narg,
                                                 meta,
                                                 action,
                                                 rawPassingOption,
                                                 rawIngestionOption,
                                                 helpFlags,
                                                 flags,
                                                 choices,
                                                 parseFun,
                                                 help,
                                                 defaultVal,
                                                 constVal,
                                                 required);

    END_RCPP
    return wrap(R_NilValue);
  }
  SEXP create_info2(SEXP listInfo){
    BEGIN_RCPP
    make_argument_info_from_column_list(listInfo);
    return wrap(R_NilValue);
    END_RCPP
  }
  SEXP create_info3(SEXP listInfo){
    BEGIN_RCPP
    make_argument_info_from_row_list(listInfo);
    return wrap(R_NilValue);
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
  // narg
  {"narg_create", (DL_FUNC) &narg_create, 2},
  {"narg_add", (DL_FUNC) &narg_add, 2},
  {"narg_close", (DL_FUNC) &narg_close, 1},
  {"narg_getnarg", (DL_FUNC) &narg_getnarg, 1},
  {"narg_getv", (DL_FUNC) &narg_getv, 1},
  {"narg_getc", (DL_FUNC) &narg_getc, 1},
  {"narg_geterr", (DL_FUNC) &narg_geterr, 1},
  {"narg_getname", (DL_FUNC) &narg_getname, 1},

  // convert
  {"convert", (DL_FUNC) &_convert, 1},
  {"convertList", (DL_FUNC) &_convertList, 1},

  // raw_argument
  {"raw_argument_create", (DL_FUNC) &_raw_argument, 3},
  {"raw_argument_add", (DL_FUNC) &_raw_argument_add, 2},
  {"raw_argument_digest", (DL_FUNC) &_raw_argument_digest, 1},

  // pfunc
  {"do_call", (DL_FUNC) &do_call, 2},
  {"do_call2", (DL_FUNC) &do_call2, 2},
  {"execute_pfunc", (DL_FUNC) &execute_pfunc, 3},

  //printfunc
  {"print_external", (DL_FUNC) &print_something_external, 2},
  {"print_internal", (DL_FUNC) &print_something_internal, 2},

  // argument_locator
  {"argument_locator_test", (DL_FUNC) &argument_locator_test, 2},

  // Raw_container
  {"create_raw_container", (DL_FUNC) &create_raw_container, 5},
  {"raw_container_add_vector", (DL_FUNC) &raw_container_add_vector, 2},
  {"raw_container_add_string", (DL_FUNC) &raw_container_add_string, 2},
  {"raw_container_extract", (DL_FUNC) &raw_container_extract, 1},

  //{"create_info", (DL_FUNC) &create_info, 14},


  {"make_argument", (DL_FUNC) &make_argument, 14},
  {"check_argument_single", (DL_FUNC) &check_argument_single, 1},
  {"make_argument_multiple", (DL_FUNC) &make_argument_multiple, 1},
  {"check_argument_multiple", (DL_FUNC) &check_argument_multiple, 1},

  {NULL, NULL, 0}
};
// Register the pointers as Dynamic Symbols for R.
extern "C" void R_init_cmdline_arguments(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, (Rboolean)FALSE);
}
