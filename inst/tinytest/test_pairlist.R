

# Test pairlist.h

Rcpp::sourceCpp("cpp/pairlist.cpp")

expect_equal(test_pairlist(list(1, 30), rep), do.call(rep, list(1, 30)))

expect_identical(test_pairlist(list(formula = mpg ~ hp, data = mtcars), lm),
                 do.call(lm, list(formula = mpg ~ hp, data = mtcars)))
