

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
#' @param
#' @param
#'
#'
#'
#' @note
#'
#'
add_argument <- function(parser, name, narg, action = 'store', s){
  # If name is a list, we assume it contains all argument parameters
  if(is.list(name)){

    return(NULL)
  }
  # if it does not, we have to go through and check if each is missing in R.
  if(missing(narg))
    stop("cannot add an argument without knowing narg")
  if(missing(action))
    stop("cannot add argument without an action")
  if(missing(s))
    stop("hello world")

}

s <- function(){

}
