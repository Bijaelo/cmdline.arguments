

#' Extract `parser.func` from an argument
#'
#' @description simple method for extracting the cmdline arguments passed to a
#' specific argument prior to being parsed.
#'
#' @param x an object of class `cmd_arg` or `cmd_parser`
#' @param ... further parameters passed to method
#'
#' @returns the return value depends on the value of `parser.func.opts`.
#' The most common types are
#' 1. Vector, if vectorized = TRUE and input type is vector
#' 2. List if vectorized = TRUE and type is list or vectorized = FALSE
getunparsedargs <- function(x, ...)
  UseMethod("getunparsedargs")

#' @describeIn getunparsedargs method for objects of class `cmd_arg`
getunparsedargs.cmd_arg <- function(x, ...)
  .Call("_getunparsedargs", extract_ptr(x))

#' @describeIn getunparsedargs method for objects of class `cmd_parser`
getunparsedargs.cmd_parser <- function(x, ...)
  .Call("_getunparsedargs_parser", extract_ptr(x))
