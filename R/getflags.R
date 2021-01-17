
## S3 methods
# FIXME: needs documentation for getflags, getsflag, and so forth.


#' extract all flags
#'
#' @description This method can be used to extract the flag values of either a
#' single argument or all arguments provided to a parser. This interface is
#' primarily thought to be useful for testing and debugging purposes.
#'
#' @param x an object of either class cmd_arg or cmd_parser
#' @param ... further arguments passed the specific method
#'
#' @returns If x is a single argument, a character vector of argument names
#' if x is a cmd_parser, a list is returned with plausible entries "flags",
#' "positional" and "named" for flag, positional and named arguments
#' respectively.
#'
#' @family parsers
getflags <- function(x, ...)
  UseMethod("getflags")

#' @describeIn getflags method for class "cmd_arg"
getflags.cmd_arg <- function(x, ...)
  .Call("_getflags", extract_ptr(x))

#' @describeIn getflags method for class "cmd_parser"
getflags.cmd_parser <- function(x, ...)
  .Call("_parser_getflags", extract_ptr(x))

#' extract the smallest flag
#'
#' @description This method can be used to extract the smallest flag within a
#' single argument or all of the smallest flag within a parser. This interface
#' is primarily thought to be useful for testing and debugging purposes.
#'
#' @param x the object to extract values from (usually cmd_arg or cmd_parser)
#' @param ... further parameters passed to the specific method
#'
#' @returns If x is a single argument, a character vector of argument names
#' if x is a cmd_parser, a list is returned with plausible entries "flags",
#' "positional" and "named" for flag, positional and named arguments
#' respectively.
#'
#' @family parsers
getsflag <- function(x, ...)
  UseMethod("getsflag")

#' @describeIn getsflag method for class "cmd_arg"
getsflag.cmd_arg <- function(x, ...)
  .Call("_getsflag", extract_ptr(x))

#' @describeIn getsflag method for class "cmd_parser"
getsflag.cmd_parser <- function(x, ...)
  .Call("_parser_getsflag", extract_ptr(x))

#' extract the largest flag
#'
#' @description This method can be used to extract the largst flag within a
#' single argument or all of the largest flag within a parser. This interface
#' is primarily thought to be useful for testing and debugging purposes.
#'
#' @param x the object to extract values from (usually cmd_arg or cmd_parser)
#' @param ... further parameters passed to the specific method
#'
#' @returns If x is a single argument, a character vector of argument names
#' if x is a cmd_parser, a list is returned with plausible entries "flags",
#' "positional" and "named" for flag, positional and named arguments
#' respectively.
#'
#' @family parsers
getlflag <- function(x, ...)
  UseMethod("getlflag")

#' @describeIn getsflag method for class "cmd_arg"
getlflag.cmd_arg <- function(x, ...)
  .Call("_getlflag", extract_ptr(x))

#' @describeIn getsflag method for class "cmd_parser"
getlflag.cmd_parser <- function(x, ...)
  .Call("_parser_getlflag", extract_ptr(x))
