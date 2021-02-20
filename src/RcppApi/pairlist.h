

/* RcppApi::pairlist
 *
 * Adds special handling for pairlist<ArgumentList> ArgumentList t1
 *
 * Grows a pairlist from ArgumentList of length n
 */

#ifndef cmdline_pairlist
#define cmdline_pairlist

#include "RcppApi/ArgumentList.h"
#include <Rcpp.h>
#include <Rinternals.h>
namespace Rcpp{
  inline SEXP pairlist(Rcpp::ArgumentList t1){
    // code mostly taken from do_docall
    // Not sure why this one is complaining about it not being a union type?
    R_xlen_t n = t1.size();
    SEXP c, call;
    PROTECT(c = call = Rf_allocList(n));
    // t1.names() might return R_NilValue making Shield<SEXP> ambigous
    SEXP names;
    PROTECT(names = t1.names());
    auto ti = t1.begin();
    // PROTECT might be unnecessary, as attr are stored as Shield<SEXP>?
    if(Rf_length(names) == 0){
      for(R_xlen_t i = 0; i < n; i++, ti++){
        SETCAR(c, *ti);
        c = CDR(c);
      }
    }else{
      for(R_xlen_t i = 0; i < n; i++, ti++){
        SETCAR(c, *ti);
        // PROTECT not needed here
        SEXP namei = STRING_ELT(names, i);
        // Test for NULL and nullstring.
        if(namei != R_NilValue && CHAR(namei)[0] != '\0')
          SET_TAG(c, Rf_installTrChar(namei));
        c = CDR(c);
      }
    }
    UNPROTECT(2);
    return(call);
  };
}
#endif
