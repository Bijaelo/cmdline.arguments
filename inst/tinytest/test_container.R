if(is.null(Sys.getenv('CMD_ARG_ALL_TEST')))
  exit_file("Set CMD_ARG_ALL_TEST environment variable to run these tests in this file.")

# Tinytest for <cmd_args/parser/argument/container.h>

Rcpp::sourceCpp("cpp/container.cpp")

ptr1 <- test_container_initializer(FALSE, TRUE)

# 2 and 3 should be identical.
ptr2 <- test_container_initializer(FALSE, FALSE)

ptr3 <- test_container_default_initializer()

arg1 <- list(name = 'test',
             flags = c('-t', '--test'),
             meta = NULL,
             action = 'store',
             option = 'i',
             choices = NULL)
arg2 <- list(name = "foo",
             flags = '--foo',
             meta = 'Fav',
             action = 'store_true',
             option = 'v',
             choices = NULL)

expect_silent(test_add_argument(ptr1,
                                arg1[["name"]],
                                arg1[["flags"]],
                                arg1[["meta"]],
                                arg1[["action"]],
                                arg1[["option"]],
                                arg1[["choices"]]))

expect_silent(test_add_argument(ptr2,
                  arg1[["name"]],
                  arg1[["flags"]],
                  arg1[["meta"]],
                  arg1[["action"]],
                  arg1[["option"]],
                  arg1[["choices"]]))

expect_silent(test_add_argument(ptr3,
                  arg1[["name"]],
                  arg1[["flags"]],
                  arg1[["meta"]],
                  arg1[["action"]],
                  arg1[["option"]],
                  arg1[["choices"]]))

expect_silent(test_add_argument(ptr1,
                  arg2[["name"]],
                  arg2[["flags"]],
                  arg2[["meta"]],
                  arg2[["action"]],
                  arg2[["option"]],
                  arg2[["choices"]]))

expect_silent(test_add_argument(ptr2,
                  arg2[["name"]],
                  arg2[["flags"]],
                  arg2[["meta"]],
                  arg2[["action"]],
                  arg2[["option"]],
                  arg2[["choices"]]))

expect_silent(test_add_argument(ptr3,
                  arg2[["name"]],
                  arg2[["flags"]],
                  arg2[["meta"]],
                  arg2[["action"]],
                  arg2[["option"]],
                  arg2[["choices"]]))


expect_silent(test_add_argument(ptr1,
                  arg2[["name"]],
                  arg2[["flags"]],
                  arg2[["meta"]],
                  arg2[["action"]],
                  arg2[["option"]],
                  arg2[["choices"]]))

expect_error(test_add_argument(ptr2,
                  arg2[["name"]],
                  arg2[["flags"]],
                  arg2[["meta"]],
                  arg2[["action"]],
                  arg2[["option"]],
                  arg2[["choices"]]), pattern = 'Duplicate flag')
expect_error(test_add_argument(ptr3,
                  arg2[["name"]],
                  arg2[["flags"]],
                  arg2[["meta"]],
                  arg2[["action"]],
                  arg2[["option"]],
                  arg2[["choices"]]), pattern = 'Duplicate flag')


expect_equal(test_nflags(ptr1), 3)
expect_equal(test_nflags(ptr2), 3)
expect_equal(test_nflags(ptr2), 3)

expect_equal(test_narguments(ptr1), 3)
expect_equal(test_narguments(ptr2), 2)
expect_equal(test_narguments(ptr2), 2)

expect_true(all(sapply(list(test_getflags(ptr1),
                            test_getflags(ptr2),
                            test_getflags(ptr3)),
                       function(x)all(x %in% c('--foo', '--test', '-t')))))



