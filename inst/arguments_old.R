
# Maybe change flags to "..." argument similar to argparse and limit the length
# internally? Makes a lot of sense, while being a slightly slower possibility.


# add description for flag, required, parse.func, parse.func.args, const, default
#
# create example of const, default, setting arguments, parsing arguments and
# errors when working with different actions.



#' Method for generating a single argument
#'
#' @description This method provides an interface for creating a single
#' argument. While it is not recommended for use in production ready code,
#' this allows any user to illustrate the creation of individual parser
#' components and may potentially speed up interpretation for scripts of 1 - 2
#' arguments by avoiding the need for an interface to the R6 class. However,
#' it is still recommended to interact with the standard interface outside of
#' visualizing the methodology.
#'
#' @param ... argument flags / names. (only used by parser objects)
#' @param name name of the argument. This becomes the name of any parsed
#' argument and is used for extracting by name. If null the longest flag argument is used.
#' @param narg argument for deciding the behaviour when passed multiple
#' arguments. See details for more information.
#' @param action specification for how to handle arguments. See detailss for more information.
#' @param flag logical, is the argument a flag argument (`TRUE`/`FALSE`)?
#' @param required logical, is the argument required (`TRUE`/`FALSE`)?
#' @param parse.func function used to parse input. See details.
#' @param parse.func.args arguments passed to `parse.func`. Defaults to `identity`.
#' @param const constant value to hold for storing upon various actions. See details for more information.
#' @param default value to be returned when no arguments are passed.
#'
#' @details For those familiar with the [argparse](https://docs.python.org/3/library/argparse.html)
#' package in python the \code{narg} argument works in a very similar fashion to
#' \code{narg} in their package with a few extensions.
#' The valid options are "?", "*", "+", "N", "N+" and "N-". The first four works
#' exactly the same, while the latter two provides some addditional options for
#' "at least" and "at most" N arguments.
#' 1. \code{narg = "?"} indicates that we accept either 0 or 1 argument.
#' 2. \code{narg = "*"} indicates that any number of arguments (0, 1, 2, ...)
#' are accepted
#' 3. \code{narg = "+"} indicates that require at least 1 argument.
#' 4. \code{narg = "N"} (where N is an integer) indicates that we accept N and
#' only N arguments
#' 5. \code{narg = "N+"} indicates that we accept N or more arguments
#' but not less
#' 6. \code{narg = "N-"} indicates that we accepted N or less arguments.
#'
#' Note that an argument with \code{narg = "+"} and \code{required = FALSE} is
#' equivalent to specifying \code{narg = "*"}. For \code{narg = "N"} and
#' \code{narg = "N+"} specifying \code{required = FALSE} will have an effect
#' similar to `"?"` and `"+"` but starting from `"N"`. Eg. we require either `0`
#' arguments, `"N"` or `"N+"` arguments.
#' As a slight note this does mean that `narg = 1` is equivalent to `narg = "?"`
#' as the behaviour is fully controlled by `required = TRUE/FALSE`.
#'
#' If the argument is a flag (indicated by setting `flag = TRUE`) both `narg`
#' and `required` are ignored internally.
#'
#' FIX ME:: add description of remaining arguments:
#'
#' @note if nchar(sflag) > nchar(lflag) the arguments are switched internally.
#'
#' @examples
#' # Create a simple argument and extract each available field.
#' f <- createArgument('h', 'help', 'help', 3, "store_value", TRUE, FALSE)
#' getflags(f)
#' getsflag(f)
#' getlflag(f)
#' getnarg(f)
#' getnargparsed(f)
#' isflag(f)
#' isrequired(f)
#' getname(f)
#' getparsefun(f)
#' # provide argument with stuff to parse
#' setUnparsedArgs(f, c('a', 'b', 'c'))
#' getunparsedargs(f)
#' parseargs(f)
#'
#' #
#'
#'
createArgument <- function(...,
                           name,
                           narg,
                           action = 'store_value',
                           flag = FALSE,
                           required = TRUE,
                           parse.func = identity,
                           parse.func.args = list(),
                           const = NULL,
                           default = NULL
                           ){
  if(!is.character(name) || length(name) > 1)
    stop(sprintf("[arg: %s] multiple names is not allowed", paste0(names, collapse = ',')))
  # We gotta ensure that flag and required are not null types and only contain
  # a single value. This is slightly more difficult from C++ (aka: I haven't figured out how yet)
  # The suggested method by R-ext is also to do this in R itself.
  if(!is.logical(flag) ||
     !is.logical(required) ||
     length(flag) > 1 ||
     length(required) > 1 ||
     is.na(flag) ||
     is.na(required))
    stop(sprintf('[arg: %s] flag and required must be either TRUE or FALSE'))
  # Maybe we should parse flag right here?
  flags <- unlist(list(...))
  if(!is.character(flags))
    stop(sprintf("[arg: %s] flags/names must be one or two character strings.", name))
  res <- .Call("_createArgument", flags, name, as.character(narg),
               action, flag, required, parse.func, parse.func.args,
               const,
               default)
  class(res) <- c("cmd_arg", class(res))
  res
}




