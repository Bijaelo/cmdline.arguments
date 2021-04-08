

/* macro header
 *
 * Contains a few C-style wrappers for consistent handling of the R C api.
 *
 * These are likely to be replaced in the future with a similar structure as
 * used in the Rcpp api. But for now, there are some inefficiencies in the Rcpp
 * api (subestting specifically) that calls for API specialization.
 */
#ifndef cmd_args_macros
#define cmd_args_macros

namespace cmd_args::utils::macros{
  // Macros for slightly simpler protection and unprotection.
  #ifndef BEGIN_RCAPI
  #define BEGIN_RCAPI                                            \
    int nprot___ = 0;
  #endif

  #ifndef PROTECT_CMD
  #define PROTECT_CMD(what) do{;                                 \
    PROTECT(what);                                               \
    nprot___++;                                                  \
  } while (0)                                                    \

  #endif

  #ifndef END_RCAPI
  #define END_RCAPI                                              \
    if(nprot___ > 0){                                            \
      UNPROTECT(nprot___);                                       \
      nprot___ = 0;                                              \
    }
  #endif


  // Macro for generating NA string vector.
  // The protect "should" be unnecessary. But rather safe than sorry.
  #ifndef repn
  #define repn(where, what, n) do {                              \
    PROTECT_CMD(where = Rf_allocVector(STRSXP, n));              \
    ptr = STRING_PTR(where);                                     \
    for(R_xlen_t i = 0; i < n; i++, ptr++)                       \
      *ptr = what;                                               \
  } while (0)                                                    \

  #endif


  // Small wrapper for making a list and protecting it.
  #ifndef nList
  #define nList(where, n) do{                                    \
    where = PROTECT(Rf_allocVector(VECSXP, n));                  \
    nprot___++;                                                  \
  } while (0);                                                   \

  #endif

}

#endif
