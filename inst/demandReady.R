

#' Demand that an object is ready for parsing
#'
#' @description This function is a simple function to test whether a specific
#' argument or all arguments in a parser are ready for parsing. This is mainly
#' thought to be useful for visualization as it is not used internally.
#'
#' @param x a parser object that should be ready for parsing
#' @param ... further parameters passed to method
#'
#' @return NULL invisible
demandReady <- function(x, ...)
  UseMethod("demandReady")

#' @describeIn demand method for objects of type `cmd_arg`
demandReady.cmd_arg <- function(x, ...){
  .Call("_demandReady", x)
}

#' @describeIn demand method for objects of type `cmd_parser`
demandReady.cmd_parser <- function(x, ...){
  .Call("_demandReady", x)
}
