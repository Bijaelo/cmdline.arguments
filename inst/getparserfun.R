

#' Extract `parser.func` from an argument
#'
#' @description simple method for extracting the `parser.func` used for a single
#' argument or all parser functions in a parser class.
#'
#' @param x an object of class `cmd_arg` or `cmd_parser`
#' @param ... further parameters passed to method
#'
#' @returns if x is an object of `cmd_arg` a single functon otherwise a named
#' list of functions
getparsefunc <- function(x, ...)
  UseMethod("getparsefunc")

#' @describeIn getparsefunc method for objects of class `cmd_arg`
getparsefunc.cmd_arg <- function(x, ...)
  .Call("_getparsefun", extract_ptr(x))

#' @describeIn getparsefunc method for objects of class `cmd_parser`
getparsefunc.cmd_parser <- function(x, ...)
  .Call("_getparsefun_parser", extract_ptr(x))
