# cmdline.arguments, providing a fully fledge commandlien argument parser specialized for R's data structures.
Welcome to the `cmdline.arguments` package source repo. The home for R's own cmdline argument parser. As of 2021-03-01 this package is still in the early stages of it's development cycle, but once finished the intention is to provide the ultimate experience to the R community.

This package seeks to provide an answer to the problem "I want a standardized parser for commandline arguments for the R software language, allowing to parse **any** and all kinds of arguments to any and all of R's data types". Several packages already attempt to answer this question an fail, while some provide a more minimal solution. In the cmdline.arguments project we seek to not only copy another implementation, but instead let all other implementation inspire the development of this package extending where we find problems that occur in daily production code, to ensure reproducibility and simplicity for the end user, while allowing for more detailed customization for both individual and overall argument parsing. 

The package is developed in R and currently requires C++17 and R-4.0.0 to function. This may change in the future, but is not an overall goal. We are always looking for contributions from experience R and Rcpp users as well as suggestions and bugs from the community so hit right into the issue tab and make your voice known!

# Where are we in the development process before release?
The development process of cmdline.arguments is an iterative cycle, so any or all of the below described points may changed at any point up till version 1.0.0 (release). So while these do illustrate the release features of the current imagination of the package, do keep this in mind.

- [x] Develope the "bones" of the package 
  - [x] Implement handling for number of arguments 
    - [x] Implement and est narg class, handling options 'N', '-N', '+N', '?', '*' and '+'
  - [x] Implement and test parser function class
    - [x] Extend `Rcpp::Function::invoke` to handle "given" number of arguments 
      - Done through extending `Rcpp::pairlist` to use `class ArgumentList`
    - [x] Implement ArgumentList class 
    - [x] Test return is identical for `Rcpp::Function` and `do.call` for all input types
  - [x] Implement container for raw cmdline arguments (strings)
    - [x] Allow option for passing raw vector as  
      1. individual parameters, 
      2. a list with a single vector element
      3. a character vector
    - [x] Test and integrate with parser function class
  - [x] Implement and test commandline locater class
    - [x] Implement V1 (legacy idea)
    - [x] re-implement to comply with the idea of cmdline.arguments V2, raw_arguments and so on.
  - [ ] Integrate all aspects with the argument and parser class
- [-] Develope the "meat" of the package
  - [-] Implement and test Printer class (for help messages)
    - [x] Implement and test V1
    - [ ] Allow for "known" help argument across multiple arguments
  - [ ] Implement Argument class
    - [ ] Implement Lazy Evaluation
    - [ ] Allow different handling for `-f a -f b` and `-f a b`
  - [ ] Implement Parser class 
    - [ ] Implement Lazy Evaluation
- [ ] Develope the "skin" (API) of the package 
  - [ ] Decide upon exact format for the R api
  - [ ] Implement individual functions for interacting with single Arguments and multiple Arguments (parser class).
  - [ ] Implement R class for containing the C++ object pointers 

In addition to the checklist above it is intended t

Note that this is not a complete list, and as the package is developed a maintained diagram of the meat and bones will be updated in the drawio folder describing the individual parts in a simplified flowchart.

The intention is that the horizon to finish the above is within the year 2021 and hopefully earlier than this.

<!--
Welcome to the `cmdline.arguments` package. This package seeks to provide a fully implemented parser for commandline arguments, based on a fork of the [`argparser`][1] package version 0.6 (Thanks djhshih for his package). While several packages such as 
1. [`argparser`][1]
1. [`argparse´][2]
1. [`docopt`][3]
1. [`getopt`][4]
1. [`GetoptLong`][5]
1. [`littler`][6]
1. [`optigrab`][7]
1. [`optparse`][8]
provide various interfaces to parsing command line argument these all are limiting in their customization. The [`argparser`][1] package currently seems to provide the most customizable interface, it has various issues when it comes to [custom arguments][9], [handling of multiple arguments][10] and provides only a very restrictive interface for custom arguments compared to the [`argparse`][11] module in [`python`][12]. In addition both [`argparse`][2] and [`argparser`][1] are very slow as one relies on [`reticulate`][13] and the other relies on the [`S4`][14] interface to [`as`][15].  
This package is a project to, in time, alleviate all of these problems, while providing a more general interface which allows the user to specify argument transformations in a similar manner to the [`argparse`][11] python module.

# Installation
To install this package one must currently use the `devtools::install_github` interface as 
```R
library(devtools)
install_github('bijaelo/cmdline.arguments')
```
In the future when the package is ready, it will be available on cran and we will be able to use  `install.packages('cmdline.arguments')` to install the package.

# Quick-start instructions
Using `cmdline.arguments` is very similar to the [`argparser`][1] package. As such we can simply do the following
```R
library(dplyr)
library(cmdline.arguments)
p <- parser(help = 'This is my first parser') %>%
  add_arguments('-f', '--foo', help = 'Some arguments', nargs = '?', type = 'integer', default = c(3, 4, 5))
args <- parse_args(p)
```
One difference from the [`argparser`](1) package is that `cmdline.arguments` is based on  [`R6`](https://cran.r-project.org/package=R6) objects. As such for users who prefer a more pythonic interface can use the `$` operator
```R
p2 <- parser(help = 'This is my first parser')
p2$add_arguments('-f', '--foo', help = 'Some arguments', nargs = '?', type = 'integer', default = c(3, 4, 5))
```
# cmdline.arguments vs argparser
One very sensible question comes why one would ever use this package over the 8 options listed above. Now I will not go over the details of every package, but will make a few examples here, illustrating the differences between this package and the [`argparser`][1] package. This will also describe many of the problems that occur when using the other packages.
## Custom argument types.
When it comes to creating creating custom inputs, the package author seem to suggest parsing the arguments as one of the known method, and post-handling arguments using functions. For example if a script has an non-negative integer argument, the suggested method is to do something like
```R
ap <- argparser::arg_parser(help = 'my parser') %>%
  add_argument('-f', '--foo', type = 'integer', help = 'non-negative integer')
args <- parse_arguments(ap)
if(!is.integer(args$foo) || args$foo < 0)
  stop('foo cannot be negative')
```
This becomes tiresome if one has to do this across multiple arguments, some of which may have multiple arguments. In python we can simply define a function or class, which takes care of error conversion and error handling.
```python
def pos_int(x):
  x = float(x)
  if(x < 0 or x - int(x) > 2**(-15))
    raise ValueError('x is not a positive integer!')
  return int(x)
import argparse
p = argparse.ArgumentParser(help = 'my parser')
p.add_argument('-f', '--foo', type = pos_int, help = 'non-negative integer')
args = p.parse_arguments()
```
Something similar is possible in [`argparser`][1] using `setAs`
```R
setClass('pos.integer')
as.pos.integer <- function(x){
  x <- as.numeric(x)
  if(x < 0 || x - as.integer(x) > 2 * .Machine$double.eps)
    stop('x is not a positive inteer!')
  x
}
setAs('ANY', 'pos.integer', as.pos.integer)
```
but while this method seemingly provides a complete method, it requires users to have a more in-depth knowledge of the R class system, and as [this issue][9] illustrates the underlying code in the [`argparser`][1] package is lacking in handling of this type of function. Now in `cmdline.arguments` we have the 3 options for the `type` argument
1. As in [`argparser`][1] we can use a single string specifying a function or `as` method
1. A function similar to [`argparse`][11] in python
1. A [`R6`][16] object, that should be returned after passing the argument as the first argument in `class$new`.
Each of these methods are carefully tested, and bugs are 

## Default arguments and multiple input arguments.
In both [`argparse`][11] in python and [`argparser`][1] it is easy to specify multiple arguments using the `nargs` argument in `add_arguments`. But while [`argparse`][11] in python consistently handles this, the R equivalent is filled with errors and even the `type` argument can end up being used for the wrong `inputs` as illustrated in [this issue][10] on their bitbucket page. The error comes in the underlying code which is build using long functions which become hard to manage and debug. In `cmdline.arguments` we opt to split code into multiple objects that each are self-contained. As such if we look at the structure of `arg_parser` and the arguments within, one notices that
1. `arg_parser` is an [`R6`][16] object, meaning it is an environment, containing functions/fields 
   1. `add_argument` used to add arguments
   1. `remove_argument` used to remove arguments
   1. `parse_arguments` used to parse commandline arguments.
   1. `arguments` used to extract a list of arguments added by `add_arguments`.
1. Each argument within `arguments` is itself an `R6` object, which contains fields
   1. `name`: the name of the argument
   1. `short_name`: the short name of the argument
   1. `dest`: the destination of the output list
   1. `parser`: the function, class or method used to parse the input argument from commandline.
   1. `arg`: If `parse_argument` has been called the parsed argument, otherwise the raw argument found from `commandArgs`. 
   1. `narg`: Similar to `nargs` in `add_argument`.
   1. `help` 
1. The `help` object within each `argument` is a simple method used to build the help message for each specific argument. By default this object builds a message similar to 

## Customization of --help and messages.
Using [`argparse`][11] and [`argparser`][1] it is incridible difficult to customize how the help message is printed. I at least know of no simple method in Python and only digging deep into the `arg_parser` object itself. While few find a need to deviate from this behaviour, some may prefer more customizability or to alter how the message is build entirely.  
In `cmdline.arguments` the underlying printing methods is based on an [`R6`][16] object which contains two methods 
* `CreateMessage`: A function which takes a single vector argument of the form, `c(short_name, name, ...)` where `...` are any number of strings to be printed and stores the result in the `message` field of the object.
* `print`: A function called by the `print.help` function, which prints `message`, possible doing some pre-formatting before outputting it to console.

When building the `parser` object by default `getOption("cmdline.argparse.helpfun")` is used to create the help message, which creates a single line of input. Overwriting this option will overwrite how all input ( including '--help' ). Overwriting the `print.argument` and `print.parser` functions will let one further customize how these arguments are formatted and combined further.
-->
