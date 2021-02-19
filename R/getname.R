

#' extract name from argument or parser
#'
#' @param x object to extract the name of
#' @param ... additional arguments passed to method
#'
#' @return if x is an
#'
getname <- function(x, ...)
  UseMethod("getname")

#' @describeIn method for objects of class `cmd_arg`
getname.cmd_arg <- function(x, ...)
  .Call("_getname", extract_ptr(x))

#' @describeIn method for objects of class `cmd_parser`
getname.cmd_parser <- function(x, ...)
  .Call("_getname", extract_ptr(x))
