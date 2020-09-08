#' Commandline argument parser
#'
#' \code{cmdline.arguments} provides an interface for parsing and transforming commandline arguments.
#'
#'
NULL


#' class for the helper message of arg_parse function.
#'
#'
help <- R6Class('help',
                public = list(
                  message = NULL,
                  print = NULL,
                  intiialize = function(CreateMessage, print){

                  }
                )
)


microbenchmark::microbenchmark(cat = cat(x, sep = '\n'), writeLines = writeLines(x), times = 1e5)
