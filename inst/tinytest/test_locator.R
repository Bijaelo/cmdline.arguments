if(is.null(Sys.getenv('CMD_ARG_ALL_TEST')))
  exit_file("Set CMD_ARG_ALL_TEST environment variable to run these tests in this file.")

Rcpp::sourceCpp("cpp/locator.cpp")

###     ###
## Setup ##
###     ###

# Simple case no weirdness, except for the '--fool=34' to test if equal signs work as expected
case1 <-
  list(args = c('-f', '--fool=34', 'abc', 'dfd', 'cc', '--good', 'ad', '3', '4', '--fool', '44', '-h'),
       flags = c('-', '--'))

# Other case, weird flags supplied
case2 <-
  list(args = c('@f', '@@fool', 'abc', 'dfd', 'cc', '-g', 'ad', '3', '4', '--fool', '44', '/h'),
       flags = c('@', '@@', '-', '/', '--'))

# case with positionals
case3 <-
  list(args = c('a', 'c', 'b', '-f', '--fool', 'abc', 'dfd', 'cc', '--good', 'ad', '3', '4', '--fool', '44', '-h'),
       flags = c('-', '--'))
###      ###
## ~setup ##
###      ###

# Test initializer

XPtr1 <- test_initializer(case1[['args']], case1[['flags']])

expect_inherits(XPtr1, 'externalptr')

XPtr2 <- test_initializer(case2[['args']], case2[['flags']])

XPtr3 <- test_initializer(case3[['args']], case3[['flags']])
# Test get

expect_identical(test_get(XPtr1, '--fool'), list(c('34', 'abc', 'dfd', 'cc'), c('44')))

expect_identical(test_get(XPtr1, '--ds'), list())

expect_identical(test_get(XPtr2, '@@fool'), list(c('abc', 'dfd', 'cc')))

expect_identical(test_get(XPtr2, '/ds'), list())

expect_identical(test_get(XPtr3, 'positionals'), list(c('a', 'c', 'b')))

# test contains

expect_true(test_contains(XPtr1, '--good'))
expect_true(test_contains(XPtr2, '-g'))

# test pop

## Test returns contains "before" -> "after"
expect_identical(test_pop(XPtr2, '-g'), c(TRUE, FALSE))

# test iterate
## Iterate returns key -> list(values) pairs

expect_identical(test_iterate(XPtr2),
                 list(list('/h',list(character())),
                      list('@f', list(character())),
                      list('--fool', list('44')),
                      list('@@fool', list(c('abc', 'dfd', 'cc')))))

expect_identical(test_iterate(XPtr1),
                 list(list('-h',list(character())),
                      list('--good', list(c('ad', '3', '4'))),
                      list('-f', list(character())),
                      list('--fool', list(c('34', 'abc', 'dfd', 'cc'),
                                          c('44')))))

# Clean the environment

rm(case1, case2, case3, XPtr1, XPtr2, XPtr3)
