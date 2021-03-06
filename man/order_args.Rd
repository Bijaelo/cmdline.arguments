% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/find_args.R
\name{order_args}
\alias{order_args}
\title{Function for ordering arguments according to some string}
\usage{
order_args(args, argPos, includeNext = FALSE, lastPos = Inf)
}
\arguments{
\item{args}{commandline argument (usually from commandArgs(TRUE)).}

\item{argPos}{argument position vector, see \code{\link{find_args}}.}

\item{includeNext}{logical indicator, if TRUE the next position of argPos is included as an attribute "next"}

\item{lastPos}{The final position within args, usually length(args)}
}
\value{
integer vector that gives the numeric order of the indices in the
input vector.
}
\description{
This function only exists for optimization purposes.
The function takes an argument vector (string vector) and a position vector
(resulting form a call to find_args) and sorts the position vector according
to the argument strings. Eg, if the arguments are '-b', '-a' then the result
would be c(2, 1). If the arguments are 'c', '-b', '-a' then the result
is c(3, 2). Sorting arguments allows a single iteration over each vector
when matching the arguments.
}
\examples{
\donttest{
args <- c('efgh', '--gijs', '-abcd')
p <- c(2, 3)
attr(p, 'argLen') <- c(2, 1)
order_args(args, p)

args <- c(args, '--aacc')
p <- c(p, 4)
attr(p, 'argLen') <- c(2, 1, 2)
order_args(args, p)

args <- c(args, '--qqq')
p <- c(p, 5)
attr(p, 'argLen') <- c(2, 1, 2, 2)
order_args(args, p)
}

}
