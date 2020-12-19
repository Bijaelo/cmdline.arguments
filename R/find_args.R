#' Locate the position of arguments passed from commandline
#'
#' @param args vector of commandline arguments. Defaults to commandArgs(TRUE)
#' @param sarg short argument indicator (one letter arguments). No default
#' @param larg long argument indicator (one or more letter).
#'
#' @description This function is intended to find the position of commandline
#' arguments. Under usual circumstances this is called from other functions but
#' has been exported for the user. It is intentionally restricted to only take 2
#' possible argument indicators, but does not in itself restrict the format of
#' those argument. These restrictions should be checked before calling the
#' function. The check itself is performed in optimized c++ code.
#'
#' @note The function checks whether the argument indicator is followed by an
#' empty space and throws an error if this is the case. This follows the output
#' syntax from commandArg, which cannot have "spaces" in arguments unless quoted.
#'
#' The function does **not** check for other types of invalid arguments
#' (duplicates, invalid symbol names etc.). These checks should be caught with
#' pattern matching.
#'
#' @examples
#' # Simple argument finding
#' find_args(c('--abcd', '-d', 'hello', '--ff'), '-', '--')
#' find_args(c('--abcd', '-d', 'hello', '--ff'), '-')
#' find_args(c('--abcd', '-d', 'hello', '--ff'), --')
#' # Example with help
#' find_args('-h', '-', '--')
#' find_args('--help', '-', '--')
#' # Note that this does not find help:
#' find_args('--help', '-')
#' # Example with alternative indicator
#' find_args('/h', , '/')
#'
#' \donttest{
#' # Example of an error:
#' find_args(c('--abcd ', '-d', 'hello', '--ff'), '-', '--')
#' }
#'
#' @returns An integer vector indicating the positions that started with either
#' sarg or larg. If both are provided the integer indicates larg over sarg to
#' avoid any confusion between "-" and "--" (or equivalent).
#' The vector contains an attribute 'argLen' indicating the end position for
#' the argument indicator.
#'
#' @importFrom rlang abort
#'
#' @export
find_args <- function(args = commandArgs(TRUE), sarg, larg){
  ms <- missing(sarg)
  ml <- missing(larg)
  if(ms && ml)
    rlang::abort("Both sarg and larg is missing. At least one must be specified")
  else if(ms){
    if(length(larg) > 1)
      rlang::abort("More than one long argument was supplied.
                   Please provide only a single argument for larg.")
    # is short arg is missing
    args <- find_args_single_c(args, larg)
    attr(args, 'argLen') <- rep(nchar(larg), length(args))
    args
  }else if(ml){
    if(length(sarg) > 1)
      rlang::abort("More than one short argument was supplied.
                   Please provide only a single argument for sarg.")
    # if long arg is missing
    args <- find_args_single_c(args, sarg)
    attr(args, 'argLen') <- rep(nchar(sarg), length(args))
    args
  }else{
    if(sarg == larg)
      rlang::abort("sarg and larg are identical. Please specify different arguments!")
    if(nchar(larg) < nchar(sarg)){
      temp <- larg
      larg <- sarg
      sarg <- temp
    }
    # If both are provided
    find_args_c(args, sarg, larg)
  }
}

#' Function for ordering arguments according to some string
#'
#'
#' @param args commandline argument (usually from commandArgs(TRUE)).
#' @param argPos argument position vector, see \code{\link{find_args}}.
#'
#' @description This function only exists for optimization purposes.
#' The function takes an argument vector (string vector) and a position vector
#' (resulting form a call to find_args) and sorts the position vector according
#' to the argument strings. Eg, if the arguments are '-b', '-a' then the result
#' would be c(2, 1). If the arguments are 'c', '-b', '-a' then the result
#' is c(3, 2). Sorting arguments allows a single iteration over each vector
#' when matching the arguments.
#'
#' @return integer vector that gives the numeric order of the indices in the
#' input vector.
#'
#' @examples
#' args <- c('efgh', '--gijs', '-abcd')
#' p <- c(2, 3)
#' attr(p, 'argLen') <- c(2, 1)
#' order_args(args, p)
#'
#' args <- c(args, '--aacc')
#' p <- c(p, 4)
#' attr(p, 'argLen') <- c(2, 1, 2)
#' order_args(args, p)
#'
#' args <- c(args, '--qqq')
#' p <- c(p, 5)
#' attr(p, 'argLen') <- c(2, 1, 2, 2)
#' order_args(args, p)
order_args <- function(args, argPos){
  argPos[order(substring_c(args[argPos], attr(argPos, 'argLen')))]
}


