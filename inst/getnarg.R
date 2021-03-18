

#' Extract narg from an argument
#'
#' @description simple method for extracting the values of narg. Mainly thought
#' to be useful for debugging purposes.
#'
#' @param x an object of class `cmd_arg` or `cmd_parser`
#' @param ... further parameters passed to method
#'
#' @returns if x is an object of `cmd_arg` a single character element specifying
#' the value of narg, otherwise a named vector with `narg` for each argument in
#' the parser
getnarg <- function(x, ...)
  UseMethod("getnarg")

#' @describeIn getnarg method for objects of class `cmd_arg`
getnarg.cmd_arg <- function(x, ...)
  .Call("_getnarg", extract_ptr(x))

#' @describeIn getnarg method for objects of class `cmd_parser`
getnarg.cmd_parser <- function(x, ...)
  .Call("_getnarg_parser", extract_ptr(x))

#' Extract parsed values for narg
#'
#' @description simple method for extracing the parsed values of narg. Mainly
#' useful for debugging purposes.
#'
#' @param x an object of class `cmd_arg` or `cmd_parser`
#' @param ... further parameters passed to method
#'
#' @returns if x is an object of class `cmd_arg` a list of length 2 where the
#' first element indicates the number N and the second indicates comparison
#' operator `<`, `<=`, `==`, `>=` or `>`.
getnargparsed <- function(x, ...)
  UseMethod("getnargparsed")

#' @describeIn getnargparsed method for object class `cmd_arg`
getnargparsed.cmd_arg <- function(x, ...)
  .Call('_getnargparsed', extract_ptr(x))

#' @describeIn getnargparsed method for object class `cmd_parser`
getnargparsed.cmd_parser <- function(x, ...)
  .Call('_getnargparsed_parser', extract_ptr(x))
