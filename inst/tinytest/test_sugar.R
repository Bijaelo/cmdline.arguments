

# TinyTest for <cmd_args/utils/sugar.h>
Rcpp::sourceCpp("cpp/sugar.cpp")

## Any duplicated
x <- 1:10

expect_false(anyDuplicated_NumericVector(x))

expect_false(anyDuplicated_SEXP(x))

x <- rep(1, 10)
expect_true(anyDuplicated_NumericVector(x))

expect_true(anyDuplicated_SEXP(x))

expect_false(anyDuplicated_SEXP(LETTERS))
expect_true(anyDuplicated_SEXP(strsplit('LETTERS', '')[[1]]))

rm(x)
## allin
x <- 1:3
table <- 1:120

expect_true(allin_NumericNumeric(x, table))

expect_error(allin_SEXPSEXP(as.integer(x), as.double(table)))

expect_true(allin_SEXPSEXP(x, table))

expect_false(allin_SEXPSEXP(1:150, table))

rm(x, table)

## samelengths
expect_true(samelengths_NumericVector(1:10))

expect_true(samelengths_SEXP(lapply(1:3, function(x)seq.int(3))))

expect_false(samelengths_SEXP(lapply(1:3, seq.int)))

# is_StringOrChar

expect_true(test_is_StringOrChar('letters'))
expect_false(test_is_StringOrChar(list()))
expect_false(test_is_StringOrChar(list))
expect_false(test_is_StringOrChar(1:10))

# is_StringOrCharAndNotNA
expect_true(test_is_StringOrCharAndNotNA('letters'))
expect_false(test_is_StringOrCharAndNotNA(list()))
expect_false(test_is_StringOrCharAndNotNA(list))
expect_false(test_is_StringOrCharAndNotNA(1:10))

x <- letters
x[19] <- NA
expect_false(test_is_StringOrCharAndNotNA(x))
rm(x)

# sexp_isnull
expect_true(sexp_isnull(NULL))
expect_false(sexp_isnull(1:3))
