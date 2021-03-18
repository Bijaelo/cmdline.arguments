
#' Methods for distributing cmdline arguments to an argument or parser
#'
#' @description These method provides an interface for manual provision of
#' arguments to either a parser or a specific argument. This action is performed
#' automatically in normal circumstances. See the individual methods for more
#' information about how the arguments are handled and specific examples.
#' These methods are not intended for production code where
#' \code{parser$parse_args(args)} or \code{parse_args(parser, args)} automates
#' this action and these methods are provided for visualizing the C++
#' implementation from R.
#'
#' @param x the object to provide unparsed arguments for
#' @param args A character vector of arguments to be passed
#' @param ... further arguments passed to the specific methods
#' @seealso [setunparsedArgs.cmd_arg], [setunparsedArgs.cmd_parser]
setUnparsedArgs <- function(x, args, ...)
  UseMethod("setUnparsedArgs")

#' Method for distributing cmdline arguments to an argument
#'
#' @description when distributing arguments to a single argument no parsing is
#' performed on the argument. Eg. if you have an argument with flags `"-f"` and
#' `"--fail"` then the arguments passed are assumed to not contain the flags
#' themselves. Every element of the string is passed through `parser.func`, so
#' all cleaning needs to be performed before providing the arguments.
#'
#' @inheritParams setUnparsedArgs
#'
#' @seealso [setUnparsedArgs.cmd_parser]
setUnparsedArgs.cmd_arg <- function(x, args, ...){
  .Call("_setUnparsedArgs", extract_ptr(x), args)
  invisible()
}
#' Method for distributing cmdline arguments to a parser containing arguments
#'
#' @description when distributing arguments to a parser, parsing is not
#' instantiated before calling `parseUnparsedArgs` on either the parser or the
#' individual arguments.
#'
#' @inheritParams setUnparsedArgs
#'
#' @seealso [setUnparsedArgs.cmd_arg]
setUnparsedArgs.cmd_parser <- function(x, args, ...)
  .Call("_setUnparsedArgs_parser", extract_ptr(x), args)

