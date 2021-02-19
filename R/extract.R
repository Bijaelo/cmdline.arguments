


#' Extract Cpp object pointer from object
#'
#' @description The interface of cmdline.arguments is implemented using a
#' combination or R, Rcpp and the R-core C api. The R interface represents the
#' boiler plate and front-end interface for the user, while the backend is
#' represented by a network of C++ functions and classes to store arguments and
#' perform the actual cmdline argument parsing. While the user provides an R
#' function every effort is made to avoid unnecessary interaction with R.
#' It is not unlikely that several Internal parser functions (in time) will be
#' called or be reimplemented directly in C++ in due time, to avoid the tiny
#' overhead caused by evaluating a call from C++.
#'
#' @param x an object containing a external_ptr object
#' @param ... further parameters passed to parent methods
#'
#' @returns an external_ptr to the object in question.
#'
#' @note This method is NOT intended for usage by the end user. It is highly
#' recommended to only interface with the pointer through the standard methods
#' that has been exposed to the user. Anyone using this pointer directly should
#' spend a not insignificant amount of time consulting the source code of the
#' package.
extract_ptr <- function(x, ...)
  UseMethod("extract_ptr")

#' @describeIn extract_ptr method for class "cmd_arg"
extract_ptr.cmd_arg <- function(x, ...)
  x

#' @describeIn extract_ptr method for class "cmd_parser"
extract_ptr.cmd_parser <- function(x, ...)
  x
