

#' Create example argument_info input
#'
#' @returns list
#' @export
#'
genAInfo <- function(){
  l <- list(name = "test",
       narg = '3+',
       meta = NA_character_,
       action = 'store',
       rawPassingOption = 'individual',
       rawIngestionOption = 'individual',
       helpFlags = '-h/--help',
       flags = c('-f', '--flag'),
       choices = NULL,
       parseFun = "sum",
       help = "hello world I a an argument",
       defaultVal = NULL,
       constVal = NULL,
       required = TRUE)

}
# SEXP name,
# SEXP narg,
# SEXP meta,
# SEXP action,
# SEXP rawPassingOption,
# SEXP rawIngestionOption,
# SEXP helpFlags,
# SEXP flags,
# SEXP choices,
# SEXP parseFun,
# SEXP help,
# SEXP defaultVal,
# SEXP constVal,
# SEXP required
