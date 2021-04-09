

# Test for <cmd_args/utils/converters.h>
Rcpp::sourceCpp('cpp/options.cpp')

options <- c('individual', 'vectors', 'combine')

expect_equivalent(0:2, sapply(options, test_option, msg = "hello world"))

expect_inherits(test_option_xptr(options[1], msg = "hello world"), 'externalptr')

expect_error(test_option('kaos'))

expect_equivalent(test_option('iddaaadfsdf', msg = "hello world"), 0)

rm(options)
