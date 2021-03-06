% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/getparserfun.R
\name{getparsefunc}
\alias{getparsefunc}
\title{Extract \code{parser.func} from an argument}
\usage{
getparsefunc(x, ...)
}
\arguments{
\item{x}{an object of class \code{cmd_arg} or \code{cmd_parser}}

\item{...}{further parameters passed to method}
}
\value{
if x is an object of \code{cmd_arg} a single functon otherwise a named
list of functions
}
\description{
simple method for extracting the \code{parser.func} used for a single
argument or all parser functions in a parser class.
}
