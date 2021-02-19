###                            ###
## Test for cmdarg_matching.cpp ##
###                            ###

# here I'm testing that I'm getting the expected results from cmdarg_matching.cpp

## Test find_args

simpleArg <- seq_len(5)
attr(simpleArg, 'argLen') <- rep(1, 5)
complicatedArg <- c(1, 5, 7, 12, 14)
attr(complicatedArg, 'argLen') <- c(2, 1, 1, 2, 1)
helpFound <- -1;
no_args <- integer(0)
attr(no_args, 'argLen') <- numeric(0)
error_1 <- "unexpected space \" \" found after argument indicator at position 1"
error_2 <- "'larg' must be a single character argument"
error_3 <- "'sarg' must be a single character argument"
error_4 <- "'sarg' and 'larg' cannot be identical"
error_5 <- "'sarg' and 'larg' must be a single character arguments and at most one must be missing"



testthat::test_that("Test that \"find_args\" returns the expected output in all known cases",
                    {
                      # Standard output.
                      testthat::expect_equal(find_args(c('-a', '-b', '-c', '-e', '-f'), '-', '--'),
                                             simpleArg)
                      testthat::expect_equal(find_args(c('--test', 'a', 'b', 'c',
                                                         '-v', 'e',
                                                         '-y', 'f', 'g', 'h', 'g',
                                                         '--touch', 'me', '-p'), '-', '--'),
                                             complicatedArg)
                      testthat::expect_equal(find_args(c('--test', 'a', 'b', 'c',
                                                         '-v', 'e',
                                                         '-y', 'f', 'g', 'h', 'g',
                                                         '--touch', 'me', '-p'), '--', '-'),
                                             complicatedArg)
                      testthat::expect_equal(find_args(gsub('-', '/', c('--test', 'a', 'b', 'c',
                                                                        '-v', 'e',
                                                                        '-y', 'f', 'g', 'h', 'g',
                                                                        '--touch', 'me', '-p')), '/', '//'),
                                             complicatedArg)
                      testthat::expect_equal(find_args(gsub('-', '/', c('--test', 'a', 'b', 'c',
                                                                        '-v', 'e',
                                                                        '-y', 'f', 'g', 'h', 'g',
                                                                        '--touch', 'me', '-p')), '//', '/'),
                                             complicatedArg)
                      # help
                      testthat::expect_equal(find_args('-h', '-', '--'),
                                             helpFound)
                      # no argument passed
                      testthat::expect_equal(find_args(c(' '), '-', '--'),
                                             no_args)
                      testthat::expect_equal(find_args(c('-'), '-', '--'),
                                             no_args)
                      testthat::expect_equal(find_args(c('--'), '-', '--'),
                                             no_args)
                      testthat::expect_equal(find_args(c('1234'), '-', '--'),
                                             no_args)
                      testthat::expect_equal(find_args(c('abcdf'), '-', '--'),
                                             no_args)
                      # Error messages:
                      testthat::expect_error(find_args(c('- ', '-- ', '-h'), '-', '--'), error_1)
                      testthat::expect_error(find_args(c('- ', '-- ', '-h'), larg = c('-', '--')), error_2)
                      testthat::expect_error(find_args(c('- ', '-- ', '-h'), c('-', '--')), error_3)
                      testthat::expect_error(find_args(c('- ', '-- ', '-h'), '-', '-'), error_4)
                      testthat::expect_error(find_args(c('- ', '-- ', '-h'), c('-', '-'), '--'), error_5)
                      testthat::expect_error(find_args(c('- ', '-- ', '-h'), '-', c('--', '--')), error_5)
                      testthat::expect_error(find_args(c('- ', '-- ', '-h'), c('-', '-'), c('--', '--')), error_5)
                    })
rm(complicatedArg, error_1, error_2, error_3, error_4, error_5, helpFound, no_args, simpleArg)
