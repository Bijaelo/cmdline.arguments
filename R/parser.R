

#' argument_parser
#'
.parser <- R6Class('cmdline.parser',
                   public = list(
                     #' initialize function for cmdline.parser
                     #'
                     #'
                     #' @param name
                     #' @param text_format See \code{\link{cmdline.text.format}}
                     #' @param args_format See \code{\link{cmdline.args.format}}
                     #' @param
                     #' { # Text formatting
                     #' @param usage
                     #' @param description
                     #' @param epilog
                     #' @param formatter_class
                     #' }
                     #' { # Argument formatting.
                     #' @param prefix_chars
                     #' @param fromfile_prefix_chars
                     #' @param conflict_handler
                     #' @param allow_abbrev
                     #' @param argument_default
                     #' }
                     #' @param parents A list of cmdline.parser objects whose
                     #' arguments should also be included.
                     #' @param add_help
                     #' @param help
                     initialize = function(description,
                                           help,
                                           parse_behaviour){

                     },
                     .arguments = function(){

                     },
                     .parse_arguments(args = commandArgs(TRUE),
                                      verbose = FALSE){
                       if(missing(args))
                         stop('Missing default arguments.')
                       # Now time for parsing arguments.
                       # Split arguments and pass them to the appropriate
                       # argument class.
                     }
                   ),
                   private = list())

