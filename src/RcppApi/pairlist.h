
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

#ifndef cmdline_pairlist
#define cmdline_pairlist

#include "RcppApi/ArgumentList.h"
#include <Rcpp.h>
#include <Rinternals.h>

namespace Rcpp{
  inline SEXP pairlist(const Rcpp::ArgumentList& t1){
    // code mostly taken from do_docall
    // Not sure why this one is complaining about it not being a union type?
    R_xlen_t n = t1.size();
    SEXP c, tail;
    PROTECT(c = tail = Rf_allocList(n));
    // t1.names() might return R_NilValue making Shield<SEXP> ambigous
    const CharacterVector names(t1.names());
    Rcpp::ArgumentList::const_iterator ti = t1.begin();
    CharacterVector::const_iterator ni = names.begin();
    // PROTECT might be unnecessary, as attr are stored as Shield<SEXP>?
    if(names.size() == 0){
      for(R_xlen_t i = 0; i < n; i++, ti++, ni++){
        SETCAR(c, *ti);
        c = CDR(c);
      }
    }else{
      for(R_xlen_t i = 0; i < n; i++, ti++, ni++){
        SETCAR(c, *ti);
        // PROTECT not needed here
        SEXP namei = *ni;
        // Test for NULL and nullstring.
        if(namei != R_NilValue && CHAR(namei)[0] != '\0')
          SET_TAG(c, Rf_installTrChar(namei));
        c = CDR(c);
      }
    }
    UNPROTECT(1);
    return(tail);
  };/*
  SEXP pairlist( const Rcpp::List& t1 ){
    return grow( t1, R_NilValue ) ;
  }*/

}
#endif
