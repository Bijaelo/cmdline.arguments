

#' Parse and extract parsed values from an argument or parser
#'
#' @description main method for telling the parser to parse its arguments
#' and return them. Results are saved and is not recomputed in further calls.
#'
#'
#' @param x an object of class `cmd_arg` or `cmd_parser`
#' @param ... further parameters passed to method
#'
#' @returns return values are determined by the
parseargs <- function(x, ...)
  UseMethod("parseargs")

#' @describeIn getnarg method for objects of class `cmd_arg`
parseargs.cmd_arg <- function(x, ...)
  .Call("_parseargs", extract_ptr(x))

#' @describeIn getnarg method for objects of class `cmd_parser`
parseargs.cmd_parser <- function(x, ...)
  .Call("_parseargs_parser", extract_ptr(x))
