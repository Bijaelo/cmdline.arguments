

library(cmdline.arguments)
## Test script for argument
rm(f)
gc()
f <- createArgument('h', 'help', 'help', '3+', FALSE, FALSE, as.character)
getflags(f)
getsflag(f)
getlflag(f)
getnarg(f)
getnargparsed(f)
isflag(f)
isrequired(f)
getname(f)
getparsefunc(f)
args <- c("help", "me")
setUnparsedArgs(f, args)
# Okay here are 2 problems.
# 1: "setUnparsedArgs" does not work as expected
# 2: parseArgs fails later on.
# Fix:
# 1. figure out why setUnparsedArgs fails
# 2. insert safety check in case that "unparsed args" has length 0.
# Notes on solution:
# 1. setUnparsedArgs failed because we were not yet flag aware.
getunparsedargs(f)
parseargs(f)



.Call(cmdline.arguments:::`_do_docall`, sum, list(1,"a" = 2,3), new.env(), 'help')

.Call(cmdline.arguments:::`_do_docall`, sum, list(1,"a" = 2,3), new.env(), 'help')
