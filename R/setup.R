

.onAttach <- function(libname, pkgname){
  if(!interactive()){
    msg <-
"###############################################################################
# Welcome to the cmdline.arguments package.                                   #
# This package is intended for developing scripts executed in a scripting     #
# environment but it seems to have been loaded in an interactive environment. #
#                                                                             #
# If you are new to scripting environments we highly suggest checking out the #
# package documentation or github page for example scripts and explanations.  #
#                                                                             #
# This message will not be displayed in a non-interactive environment.        #
###############################################################################

For god sake please update this message to be properly formatted instead of this
fast shizzle.
"
    cat(msg)
  }
  # Add package options to the option list
  options('cmdline.lazy.arguments' = TRUE)
}
