
//
// ArgumentList.h: Rcpp R/C++ interface class library -- ArgumentList
//
// Copyright (C) 2020-2021 Oliver Per Madsen
//
// This file is a derivative work of the code from the R project and Rcpp
// package project, which is licensed under GPLv2 with the option of choosing a
// later version. This code is therefore also licensed under the same terms.

/* RcppApi::pairlist
 *
 * Adds special handling for pairlist<ArgumentList> ArgumentList t1
 *
 * Grows a pairlist from ArgumentList of length n
 *
 * Usage:
 * SEXP make_args(ArgumentList args){
 *    return pairlist(args); // Note the return is automatically converted to a List by Rcpp.
 * }
 *
 */

#ifndef cmdline_args_pairlist_h
#define cmdline_args_pairlist_h

#include <cmd_args/utils/ArgumentList.h>

namespace Rcpp{
  template<>
  inline SEXP pairlist(const cmd_args::utils::ArgumentList& t1){
    // code mostly taken from do_docall, but loop split in case for no names.
    Rcpp::List data = t1.data;
    R_xlen_t n = data.size();
    SEXP tail, walker;
    PROTECT(tail = walker = Rf_allocList(n));
    const SEXP names = data.names();
    if(names == R_NilValue){
      for(R_xlen_t i = 0; i < n; i++){
        SETCAR(walker, data[i]);
        walker = CDR(walker);
      }
    }else{
      // For altreps we cant use pointers, and have to substr
      if(ALTREP(names)){
        for(R_xlen_t i = 0; i < n; i++){
          SETCAR(walker, data[i]);
          const SEXP namei = ALTSTRING_ELT(names, i);
          if(namei != R_NilValue && CHAR(namei)[0] != '\0')
            SET_TAG(walker, Rf_installTrChar(namei));
          walker = CDR(walker);
        }
      }else{
        SEXP* nameptr = STRING_PTR(names);
        for(R_xlen_t i = 0; i < n; i++, nameptr++){
          SETCAR(walker, data[i]);
          const SEXP namei = *nameptr;
          if(namei != R_NilValue && CHAR(namei)[0] != '\0')
            SET_TAG(walker, Rf_installTrChar(namei));
          walker = CDR(walker);
        }
      }
    }
    UNPROTECT(1);
    return tail;
  };
}
#endif

