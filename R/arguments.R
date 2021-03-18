

# Argument script and utilities.
#  This file contains the various functions for handling an argument. In this case
#  the R side of the argument class, as well as initial checking of parameters.
#  Most of the typechecking is performed in C++, but some things like checking for
#  missing variables and handling different ways of adding multiple arguments are
#  handled on the R side, at least initially.



#' Add one or more arguments to a parser
#'
#' @description
#'
#' @returns
#'
#' @param name
#' @param flags
#' @param name
#' @param narg
#' @param action
#' @param rawPassingOption
#' @param rawIngestionOption
#' @param helpFlags
#' @param choices
#' @param parseFun
#' @param help
#' @param defaultVal
#' @param constVal
#' @param required
#'
#'
add_argument <- function(parser,
                         flags,
                         name,
                         narg,
                         action = 'store',
                         rawPassingOption = "vector_input",
                         rawIngestionOption = "combine",
                         helpFlags = '-h/--help',
                         choices = NULL,
                         parseFun = identity,
                         help = "I am a help message",
                         defaultVal = NULL,
                         constVal = NULL,
                         required = TRUE,
                         meta = NULL){
  # If name is a list, we assume it contains all argument parameters
  if(missing(name))
    stop("name must not be missing")
  if(is.list(name))
    return(.Call(make_argument_multiple, name))
  # If name has length greater than 1, then we concatenate fall elements into a
  # list and pass it along as one call.
  if(length(name) > 1)
    return(.Call(make_argument_multiple,
                 list(name = name,
                      narg = narg,
                      meta = NULL,
                      action = action,
                      rawPassingOpton = rawPassingOption,
                      rawIngestionOption = rawIngestionOption,
                      helpFlags = helpFlags,
                      flags = flags,
                      choices = choices,
                      parseFun = parseFun,
                      help = help,
                      defaultVal = defaultVal,
                      constVal = constVal,
                      required = required)))
  # If nothing else, we just try to make a single argument.
  .Call(make_argument,
        name,
        narg,
        meta,
        action,
        rawPassingOption,
        rawIngestionOption,
        helpFlags,
        flags,
        choices,
        parseFun,
        help,
        defaultVal,
        constVal,
        required
        )
}


