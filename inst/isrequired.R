
#' extract flag information from parser or argument
#'
#' @description this method provides a way indicate whether an argument is a
#' flag. A flag argument is an argument indicating whether something is true or
#' false.
#'
#' @param x object to extract flag information from.
#'
#' @returns if x is an `cmd_arg` object, true if the object is a flag argument
#' otherwise false. If x is an `cmd_parser` object then a named logical vector
#' indicating which arguments are flags and which are not.
isrequired <- function(x)
  UseMethod("isrequired")

isrequired.cmd_arg <- function(x)
  .Call("_isrequired", extract_ptr(x))

isrequired.cmd_parser <- function(x)
  .Call("_isrequired_parser", extract_ptr(x))

