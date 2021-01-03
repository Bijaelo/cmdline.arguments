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
#'
#' # Simple argument finding
#' find_args(c('--abcd', '-d', 'hello', '--ff'), '-', '--')
#' find_args(c('--abcd', '-d', 'hello', '--ff'), '-')
#' find_args(c('--abcd', '-d', 'hello', '--ff'), '--')
#' # Example with help
#' find_args('-h', '-', '--')
#' find_args('--help', '-', '--')
#' # Note that this does not find help:
#' find_args('--help', '-')
#' # Example with alternative indicator
#' find_args('/h', , '/')
#'
#' \dontrun{
#' # Example of an error:
#' find_args(c('-- ', '-d', 'hello', '--ff'), '-', '--')
#' }
#'
#' @returns An integer vector indicating the positions that started with either
#' sarg or larg. If both are provided the integer indicates larg over sarg to
#' avoid any confusion between "-" and "--" (or equivalent).
#' The vector contains an attribute 'argLen' indicating the end position for
#' the argument indicator.
#'
#' @export
find_args <- function(args = commandArgs(TRUE), sarg, larg){
  ms <- missing(sarg)
  ml <- missing(larg)
  if(ms && ml)
    stop("at least one of 'sarg' and 'larg' must be specified.")
  else if(ms){
    if(length(larg) > 1 || !is.character(larg))
      stop("'larg' must be a single character argument")
    # is short arg is missing
    args <- .Call("_find_args_single_c", args, larg)
    attr(args, 'argLen') <- rep(nchar(larg), length(args))
    args
  }else if(ml){
    if(length(sarg) > 1 || !is.character(sarg))
      stop("'sarg' must be a single character argument")
    # if long arg is missing
    args <- .Call("_find_args_single_c", args, sarg)
    attr(args, 'argLen') <- rep(nchar(sarg), length(args))
    args
  }else{
    if(identical(sarg, larg, ignore.environment = TRUE))
      stop("'sarg' and 'larg' cannot be identical")
    if(length(sarg) > 1 || length(larg) > 1 ||
       !is.character(sarg) || !is.character(larg))
      stop("'sarg' and 'larg' must be a single character arguments and at most one must be missing")
    if(nchar(larg) < nchar(sarg)){
      warning("'nchar(sarg) > nchar(larg)' reversing order")
      temp <- larg
      larg <- sarg
      sarg <- temp
    }
    # If both are provided
    .Call("_find_args_c", args, sarg, larg)
  }
}

#' Function for ordering arguments according to some string
#'
#'
#' @param args commandline argument (usually from commandArgs(TRUE)).
#' @param argPos argument position vector, see \code{\link{find_args}}.
#' @param includeNext logical indicator, if TRUE the next position of argPos is included as an attribute "next"
#' @param lastPos The final position within args, usually length(args)
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
#' \donttest{
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
#' }
#'
#' @export
order_args <- function(args, argPos, includeNext = FALSE, lastPos = Inf){
  if(includeNext){
    or <- order(.Call(`_substring_c`,
                      args[argPos],
                      attr(argPos, 'argLen')))
    ou <- argPos[or]
    attr(ou, 'next') <- c(argPos[-1], lastPos + 1)[or]
    ou
  }else
    argPos[order(.Call(`_substring_c`,
                       args[argPos],
                       attr(argPos, 'argLen')))]
}


###             ###
## !! FIX ME !!  ##
## Needs the     ##
## parser object ##
###             ###

#' Match command arguments against a list of parser arguments
#'
#' @description This function can be used to interact with the low-level
#' functionality of the cmdline.arguments package. The match_args function takes
#' a vector of commandline argmuments, parser arguments and other descriptors
#' and returns a named list of extracted value from cmdArgs. The interface is however not user-friendly.
#'
#'
#' @param cmdArgs vector of commandline arguments. Defaults to commandArgs(TRUE)
#' @param parserArgs arguments to be searched for
#' @param parserArgsRequired logical vector indicaing whether an argument is strictly required
#' @param helpArg the argument used if one seeks help with the package
#' @param sarg short argument indicator (one letter arguments). No default
#' @param larg long argument indicator (one or more letter).
#'
#' @examples
#' cmdArgs <- c('--go', 'abc', 'efd', '-g', 'my world', '--garage', 'no', '-f', '-v')
#' parserArgs <- list(c('-g', '--garage'), '--go', '-f', '-v')
#' parserArgsRequired <- rep(TRUE, 4)
#' helpArg <- '--help'
#' sarg <- '-'
#' larg <- '--'
#' match_args(cmdArgs,
#'            parserArgs,
#'            parserArgsRequired,
#'            helpArg,
#'            sarg,
#'            larg)
#' @export
match_args <- function(cmdArgs = commandArgs(TRUE),
                       parserArgs,
                       parserArgsRequired,
                       helpArg,
                       sarg,
                       larg){
  if(missing(cmdArgs) ||
     missing(parserArgs) ||
     missing(helpArg))
    stop("'cmdarg', 'parserargs' and 'helpArg' must all be supplied")
  cmdArgPositions <- find_args(cmdArgs, sarg, larg)
  # Did we have any arguments at all?
  if((n <- length(cmdArgPositions)) == 0)
    stop("FIXME! I have gotten no arguments at all and I dont know how to handle this. Please tell me this did not get into production...")
  # Did we find help?
  if((n <- length(cmdArgPositions)) == 1 && cmdArgPositions == -1)
    stop("FIXME! I have spotted help, but I dont know how to handle this yet! Please tell me this did not make it into production...")
  # Order the arguments for our optimized matching function.
  parserArgLast <- sapply(parserArgs, tail, n = 1)
  parserArgsPos <- find_args(parserArgLast, '-', '--')
  parserArgsOrder <- order_args(parserArgLast,
                                parserArgsPos)
  cmdArgPositions <- order_args(cmdArgs,
                                cmdArgPositions,
                                TRUE,
                                length(cmdArgs))
  if(missing(parserArgsRequired) ||
     length(parserArgsRequired) != length(parserArgs) ||
     !is.logical(parserArgsRequired))
    stop("parserArgsRequired should be a logical vector with length equal to the length of parserArgs.")
  .Call(`_match_args_c`,
        cmdArgs,
        cmdArgPositions - 1,
        attr(cmdArgPositions, 'next') - 2,
        parserArgs,
        parserArgsOrder - 1,
        parserArgsRequired,
        helpArg
        )
}



