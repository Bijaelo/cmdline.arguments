
Rcpp::cppFunction(code = '
SEXP new_call(){
  SEXP REALSXP_10 = PROTECT(ScalarReal(10));
  SEXP REALSXP_5 = PROTECT(ScalarReal(5));
  SEXP out = PROTECT(LCONS(install("+"), LCONS(
    REALSXP_10, LCONS(
      REALSXP_5, R_NilValue
    )
  )));
  UNPROTECT(3);
  return out;
}',
includes = c('#include <R.h>', '#include <Rinternals.h>'))


#' argument class for containing information about argumnets and associated methods
#'
#' @description
#' The
#'
#' @detail
#'
argument <- R6Class('argument',
                    public = list(
                      initialize = function(...){

                      }
                    ),
                    active = list(sarg = function(value)private$.sarg,
                                  larg = function()private$.larg),
                    private = list(".sarg" = NULL,
                                   ".larg" = NULL),
                    portable = TRUE,
                    lock_class = TRUE,
                    cloneable = FALSE,
                    )


arg <- function(sarg, larg, name, dest, ){
  .Call("gen_class", sarg)
}

arg_getsarg <- function(ptr){
  .Call("getsarg", ptr)
}
