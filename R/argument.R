


#' Argument R6 class constructor
#'
#' @param
#'
#'
#' @importFrom R6 R6Class
#'
#' @exportClass
.argument <- R6Class(
   'cmdline.argument',
   public = list(
      initialize = function(...){

      },
      arguments = list(),
      lazy = getOption('cmdline.lazy.arguments', FALSE),

   ),
   private = list(),
   portable = TRUE,
   lock_class = TRUE,
   cloneable = FALSE,
   # set parent.env to be empty, so that we do not use
   # variables stored in the global or package environment.
   parent.env = baseenv(),
)


#' Standard method for adding argument
#'
#' @inheritParams add_argument.argument
#'
#' @export
add_argument <- function(obj, ...)
   UseMethod('add_argument')


#' Underlying method for adding arguments to the R6 class "argument"
#'
#' @param obj
#' @param name
#' @param short_name
#' @param action
#' @param nargs
#' @param const
#' @param default
#' @param type
#' @param choices
#' @param required
#' @param help
#' @param metavar - Maybe this should be included? I'll have to see.
#' @param dest
#' @param - Maybe I should include some other variables.
#'
add_argument.cmdline.parser <- function(obj,
                                name,
                                short_name,
                                action,
                                nargs,
                                const,
                                default,
                                type,
                                choices,
                                required,
                                help,
                                metavar,
                                dest) {
   # Use the object method to add arguments.
   obj$add_argument(
      name,
      short_name,
      action,
      nargs,
      const,
      default,
      type,
      choices,
      required,
      help,
      metavar,
      dest
   )
   # Return the object itself to allow for piping using dplyr.
   obj
}


#' Default print method for arguments
#'
#' @param x an object of class 'argument'.
#' @param ... further arguments passed to the print method of argument.
print.argument <- function(x, ...)
   x$print(...)
