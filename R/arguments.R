

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
#' @param sflag smallest flag
#' @param lflag longest flag
#' @param name name of the argument. This becomes the name of any parsed
#' argument and is used for extracting by name. If null lflag is used instead.
#' @param narg argument for deciding the behaviour when passed multiple
#' arguments. See details for more information.
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
createArgument <- function(sflag,
                           lflag,
                           name,
                           narg,
                           flag = FALSE,
                           required = TRUE){
  # We gotta ensure that flag and required are not null types and only contain
  # a single value. This is slightly more difficult from C++ (aka: i haven't figured out how yet)
  if(!is.logical(flag) ||
     !is.logical(required) ||
     length(flag) > 1 ||
     length(required) > 1 ||
     is.na(flag) ||
     is.na(required))
    stop('flag and required must be either TRUE or FALSE')
  if(!is.character(sflag) || length(sflag) > 1)
    stop("sflag should be a single character string")
  if(!is.character(name) || length(name) > 1)
    stop("name should be a single character string")
  if(!is.character(narg) || length(narg) > 1)
    stop(sprintf("%s should be one of (%s) where N is a positive integer", "nargs",
         "\"?\", \"+\", \"*\", \"N\", \"N-\", \"N+\""))
  if(!missing(lflag)){
    if(!is.character(lflag) || length(lflag) > 1)
      stop("lflag should be a single character string")
    res <- .Call("_createArgument", sflag, lflag, name, as.character(narg), flag, required)
  }else
    res <- .Call("_createArgumentS", sflag, name, as.character(narg), flag, required)
  class(res) <- c("cmd_arg", class(res))
  res
}




