## usethis namespace: start
#' @importFrom Rcpp sourceCpp
#' @useDynLib cmdline.arguments, .registration = TRUE
## usethis namespace: end
NULL

.onUnload <- function (libpath) {
  library.dynam.unload("cmdline.arguments", libpath)
}
