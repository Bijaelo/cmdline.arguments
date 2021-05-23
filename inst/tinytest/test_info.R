if(is.null(Sys.getenv('CMD_ARG_ALL_TEST')))
  exit_file("Set CMD_ARG_ALL_TEST environment variable to run these tests in this file.")

Rcpp::sourceCpp('cpp/info.cpp')


###     ###
## Setup ##
###     ###
if(FALSE){
# Currently I am considering whether Info should be used or not.
  name
narg
meta
action
rawPassingOption
rawIngestionOption
flags
choices
parseFun
help
defaultVal
constVal
required
cargs <- list(name = 'test'
              , narg = '2'
              , meta = 'TEST'
              , action = 'store'
              , rawPassingOption = # This name needs to be changed
              , rawIngestionOption
              , flags
              , choices
              , parseFun
              , help
              , defaultVal
              , constVal
              , required)

args <- list(name = 'test',
             narg = '3+',
             )

###      ###
## ~Setup ##
###      ###
}
expect_inherits
