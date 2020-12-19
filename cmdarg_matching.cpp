#include <Rcpp.h>
#include <vector>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//
// [[Rcpp::plugins(cpp2a)]] 


/*
 * find_args finds the position of arguments given long and short args
 * reports an error for inconsistent arguments, and -1 if help was requested.
 * Otherwise an integer vector is returned, with "argLen" attribute indicating if it was a long or short arg. 
 * 
 */
// [[Rcpp::export(rng=false)]]
IntegerVector find_args_c(const std::vector<std::string> x, 
                        const std::string argS, 
                        const std::string argL){
  // Maybe int is a bad choice here.
  const R_xlen_t xn = x.size(), 
    sn = argS.length(), 
    ln = argL.length();
  int snp1 = sn + 1,
    lnp1 = ln + 1;
  // Output vector that automatically resizes. 
  std::vector<R_xlen_t> v, attr;
  // pre-allocate vector, so that it may .
  v.reserve(xn);
  attr.reserve(xn);
  // Iterate over the elements our x, locate any commandline arguments passed
  for(int i = 0; i < xn; i++){
    // Search for the arguments in x
    // Start with long arguments
    std::string xi = x[i];
    long xin = xi.length();
    if(xi.substr(0, ln) == argL){
      // check for "help"
      if(xi.substr(ln, xin) == "help")
        goto help_found;
      else if(xin > ln){
        if(xi.substr(ln, lnp1) == " "){
          Rcpp::stop("Error: Unexpected space \" \" found after argument indicator at position %i.", i + 1);
        }else{
          v.push_back(i + 1); 
          attr.push_back(ln);
        }
      }
    }else if(xi.substr(0, sn) == argS)
    {
      if(xi.substr(sn, xin) == "h")
        goto help_found;
      else if(xin > sn){
        if(xi.substr(sn, snp1) == " "){
          Rcpp::stop("Error: Unexpected space \" \" found after argument indicator at position %i.", i + 1);
        }else{
          v.push_back(i + 1);
          attr.push_back(sn);
        }
      }
    }
  }
  v.shrink_to_fit();
  attr.shrink_to_fit();
  { // This seems to be needed... wonder why. 
    IntegerVector out = wrap(v);
    out.attr("argLen") = wrap(attr);
    return out;
  }
// This  goto statement improved performance by alot for high number of arguments!
help_found:
  IntegerVector rt = {-1};
  return rt;
}

/*
 * find_args_singleSearch finds the position of arguments given an argument indicator string "arg"
 * It will check whether the argument is well handled, and whether "help" is included in the argument list.
 * 
 * Returns the position of arguments or -1 if help was found. 
 * 
 * ## Fixme: Needs to throw an error is an argument is 
 */
// [[Rcpp::export(rng=false)]]
IntegerVector find_args_single_c(const std::vector<std::string> x, 
                        const std::string arg){
  // Maybe int is a bad choice here.
  const R_xlen_t xn = x.size(), an = arg.length();
  // Output vector that automatically resizes. 
  std::vector<R_xlen_t> v;
  // pre-allocate vector, so that it may .
  v.reserve(xn);
  // Iterate over the elements our x, locate any commandline arguments passed
  for(int i = 0; i < xn; i++){
    // Search for the arguments in x
    // Start with long arguments
    std::string xi = x[i];
    long xin = xi.length();
    if(xi.substr(0, an) == arg){
      // check for "help"
      std::string xia = xi.substr(an, xin);
      if(xia == "h" || xia == "help")
        goto help_found;
      else if(xin > an){
        if(xi.substr(an, an + 1) == " "){
          Rcpp::stop("Error: Unexpected space \" \" found after argument indicator at position %i.", i + 1);
        }else
          v.push_back(i + 1);
      }
    }
  }
  v.shrink_to_fit();
  return wrap(v);
  // This  goto statement improved performance by alot for high number of arguments!
  help_found:
    IntegerVector rt = {-1};
  return rt;
}


// Faster substring function for our specific needs.
// [[Rcpp::export(rng=false)]]
CharacterVector substring_c(const std::vector<std::string> x, 
                            const std::vector<long> l){
  const R_xlen_t j = x.size();
  std::vector<std::string> o;
  o.reserve(j);
  for(R_xlen_t i = 0; i < j; i++){
    const std::string xi = x[i];
    o.push_back(xi.substr(l[i], xi.size()));
  }
  return wrap(o);
}



/*
 * Match arguments from commandArgs() and the args sought by parserArgs.
 * See the first initial comment for a longer explanation of the full walkthrough.
 * 
 * Examples (from R):
 * 
 * # Setup example pieces
 * cmdArgs <- c('--positive', 'abc', 'd', '-f', 'ef', 'hg', '-p', 'ff')
 * parserArgs <- list(c('-h', '--help'), '-f', c('-v', '--verbose'))
 * required <- c(TRUE, TRUE, FALSE)
 * 
 * argPos <- find_args(cmdArgs, '-', '--')
 * argOrder <- order_args(cmdArgs, argPos)
 * parserOrder <- order_args(sapply(parserArgs, tail, 1), 
 *                           find_args(sapply(parserArgs, tail, 1), 
 *                                     '-', '--'))
 * argmatch <- match_args_c(cmdArgs, 
 *                          parserArgs, 
 *                          argOrder, 
 *                          parserOrder, 
 *                          required, 
 *                          argPos,
 *                          '--help')
 * 
 * # Version that fails due to required
 * required_v2 <- c(TRUE, TRUE, TRUE)
 * 
 * argmatch <- match_args_c(cmdArgs, 
 *                          parserArgs, 
 *                          argOrder, 
 *                          parserOrder, 
 *                          required, 
 *                          argPos,
 *                          '--help')
 * 
 * # Should fail with this:
 * required_v3 <- c(TRUE, FALSE, TRUE)
 * # Should work with this
 * required_v4 <- c(TRUE, FALSE, FALSE)
 * 
 *                          
 * match_args_c
 * 
 * 
 * cmdArgs contains all commandline arguments passed or just the observed positions?
 * If only the observed positions how do we locate the "final" position of cmdArgs?
 * If all then how do we ensure cmdArgsOrder has the same length as cmdArgs?
 * 
 * Currently I do not take this into account so there is definitely an error here!.
 */
// [[Rcpp::export(rng=false)]]
Rcpp::List match_args_c(const std::vector<std::string> cmdArgs, 
                        const List parserArgs,
                        const IntegerVector cmdArgsOrder,
                        const IntegerVector parserArgsOrder,
                        const LogicalVector parserArgsRequired,
                        const IntegerVector cmdArgsIndex,
                        const std::string helpArg){
  /* Assume cmdArgsOrder and parserArgsOrder are correctly sorted.
   * Iterate over cmdArgs, in the order of cmdArgsOrder
   * Iterate over parserArgs in the order of parserArgsOrder
   * if cmdArgs does not match parserArgs check that parserArgsRequired is False
   * for the specific index
   * If it is not then throw error. If it is, skip parserArgs(i) and check next arg.
   * If it did not match any arg throw error.
   * 
   * to detect whether an argument "matches" a parserArg, compare the exact 
   * string length. If it matches check the next symbol (if it exists). It may 
   * only be a "=" and nothing else. Otherwise it is "not match".  
   * 
   * What should be returned? Probably the parserArgsOrder and 0? 
   * Well.. If it does not exist then we should just throw an error anyway. 
   * unless it is part of the last arg? hmmm.... 
   * 
   * I need to think about how I'll handle "infinite" args...
   * Probably I'll be coming back once I figure that out..
   * Lets start by focusing on just matching "finite" arguments 
   * and break if it does not match.
   * 
   * First of all how do we fix that "cmdArgs" is a vector of vectors?. 
   * That is so simple however! vector<vector<string>> is the way to go!
   * 
   * Lets start by ignoring argument count and add this in later..
   * Maybe we'll split the function up into smaller parts later as well.. 
   * But then we'll be using pointers to avoid copying the objects.
   */
  R_xlen_t pn = parserArgs.size(), j = 0;
  List output;
  // Pre-create output list. It is a list of lists, which each contain ranges for start and end value of cmdArgs
  // Iterate over cmdArgs (change to iterate over the order of cmdArgs)
  const long cmdArgsIndexMax = max(cmdArgsIndex);
  bool found = false;
  for(auto i : cmdArgsOrder){
    auto cArgi = cmdArgs[i];
    // Iterate over parserArgs, but dont "double" check an argument 
    // They should be ordered.
    while(j < pn){
      // Iterate over the elements within the specific parser (short/long args)
      const R_xlen_t pi = parserArgsOrder[j];
      const std::vector<std::string> ji = parserArgs[pi];
      const std::string nm = ji[0];
      output.push_back(List::create(), nm);
      for(auto jii : ji){ 
        const long jiinchar = jii.size(), cArgi_n = cArgi.size();
        Rcpp::Rcout << "i: " << i << " j: " << j << " jii: " << jii << "\n" <<
                    "pi: " << pi << " cArgi: " << cArgi << " jiinchar: " << jiinchar << std::endl;
        
        if(
          cArgi_n >= jiinchar && // Sanity check: Can we take a substring as long as our current parserArg?
          ((jii == cArgi.substr(0, jiinchar) &&
             cArgi_n == jiinchar) || 
           (cArgi_n > jiinchar && 
             cArgi.at(jiinchar + 1) == '='))){
          // We found a match! Here we need to add it to our output 
          // !!!FIX ME!!!
          IntegerVector o(2);
          o.push_back(cmdArgsIndex[i] + 1, "start");
          // If we're at the last element, push the the index !!! FIX ME !!! Exactly what should the comparison be?
          if(cmdArgsIndex[i] == cmdArgsIndexMax)
            o.push_back(R_PosInf, "end");
          else
            o.push_back(cmdArgsIndex[i + 1], "end");
          // Slightly alternative way of appending to the list. Avoids proxy problem.. maybe.
          List clist = output[nm];
          clist.push_back(o);
          found = true;
          goto nextCmdArg; // skip parser iterators and move to next cmdArgs.
        }
      }
      if(parserArgsRequired[pi] && !found)
        stop("Error: Required parser argument, \"%s\", is missing.", ji[0]);
      j++;
      found = false;
    }
    /*
     *  If we were not pushed to "nextCmdArg" we conclude, that we have tried
     *  to match the argument to all values cmdArgs and have been unsuccesful. 
     */
    stop("Error: Unknown commandline argument \"%s\" provided. See %s for more information.", 
         cArgi, helpArg);
nextCmdArg:
{} // braces here only exist to avoid compiler error.
  }
  return output;
}


// [[Rcpp::export(rng=false)]]
Rcpp::List match_args_c_v2(const std::vector<std::string> cmdArgs, 
                        const std::vector<std::vector<std::string>> parserArgs,
                        const IntegerVector cmdArgsOrder,
                        const IntegerVector parserArgsOrder,
                        const LogicalVector parserArgsRequired,
                        const IntegerVector cmdArgsIndex,
                        const std::string helpArg){
  R_xlen_t pn = parserArgs.size(), j = 0, cn = cmdArgsOrder.size();
  Rcout << pn << std::endl;
  std::list<std::list<std::vector<int>>> output;
  // Pre-create output list. It is a list of lists, which each contain ranges for start and end value of cmdArgs
  // Iterate over cmdArgs (change to iterate over the order of cmdArgs)
  const long cmdArgsIndexMax = max(cmdArgsIndex);
  bool foundParserMatch = false, found = false;
  std::string ji;
  for(auto i : parserArgsOrder){
    // Prepare arguments to be inserted
    std::list<std::vector<int>> output_I;
    const std::vector<std::string> pi = parserArgs[i]; // move this into the next loop!
    const int pin = pi.size();
    int pini = 0; // pini: Iterator over the elements in pi. Avoids iterating twice.
    // Iterate over the elements of our commandArgs
    while(j < cn){
      Rcout << "j: " << j << " cn: " << cn << std::endl;
      ji = cmdArgs[cmdArgsOrder[j]];
      const long jinchar = ji.size();
      // Iterate over the parser arguments
      while(pini < pin){
        Rcout << "pini: " << pini << " pin: " << pin << std::endl;
        // Test if the argument matches the parser:
        const std::string pii = pi[pini];
        const long piinchar = pii.size();
        /* 
         * If the cmdArg has the same length as the parser, match entire string
         * If the cmdArg is longer, match a substring and check that the next 
         * character is a "=" (eg: we have an inline argument). 
         */
        
        // Print out our coefficients 
        std::string txt = (jinchar == piinchar) ? "true" : "false", 
          txt2 = (pii == ji) ? "true" : "false";
        
        Rcpp::Rcout << "jinchar: " << jinchar << " piinchar: " << piinchar
                    << "\npii: " << pii << " ji: " << ji 
                    << "\npii == ji: " << txt2
                    << "\njinchar == piinchar: " << txt << std::endl;
        if((jinchar == piinchar && 
             pii == ji) || 
           (jinchar > piinchar && 
             pii == ji.substr(0, piinchar) && 
             ji.at(piinchar) == '=')
        ){
          // We found an argument. 
          found = true;
          foundParserMatch = true; // Maybe we can remove 
          // Generate the output range and append it to our output vector of lists of vectors.
          std::vector<int> elem(2);
          elem[0] = cmdArgsIndex[i]; // Index for the current
          if(cmdArgsIndex[i] == cmdArgsIndexMax)
            elem[1] = R_PosInf;
          else
            elem[1] = cmdArgsIndex[i + 1];
          output_I.push_back(elem);
          break;
        }else{
          pini++;
        }
      }
      if(!found)
        break;
      j++;
      if(pini > pin)
        break;
    }
    if(parserArgsRequired[i] && !foundParserMatch)
      stop("Error: Required parser argument, \"%s\", is missing. See %s for more information about available arguments.", pi[0], helpArg);
    if(foundParserMatch)
      output.push_back(output_I);
  }
  if(!found && j < cn)
    stop("Error: Unknown commandline argument \"%s\" provided. See %s for more information about available arguments.", 
         ji, helpArg); 
  
  return wrap(output);
}


/* Perform a single match for a commandArg against a parserArg. 
 * Use referenced objects to avoid unnecessary copying. 
 */

inline bool cmdArg_matches_parserArg(const std::string& cmdArg, 
                                     const std::string& parserArg,
                                     const R_xlen_t& cmdArgN){
  const int cmn = cmdArg.size(), pan = parserArg.size();
  return (((cmn == pan) && (cmdArg == parserArg)) || 
          ((cmn > pan) && (cmdArg.substr(0, pan) == parserArg) && (cmdArg.at(pan) == '='))); 
}


// Generate a vector of indices over a specific argument
inline std::vector<R_xlen_t> get_cmdArg_range(const R_xlen_t& cmdPosIndex, 
                                              const R_xlen_t& cmdMaxPosition, 
                                              const std::vector<R_xlen_t>& cmdArgPositions){
  // This may have to be done in 2 lines:
  return {cmdArgPositions[cmdPosIndex], 
          (cmdPosIndex == cmdMaxPosition) ? 2147483647 : cmdArgPositions[cmdPosIndex + 1] - 1};
}

/* Iterate over the parserArgs for a given cmdArg. Return a boolean indicating the outcome
 * false: Reached end of parserArg and did not find a match
 * true: Value was matched and appended
 */
inline bool iterate_over_parserArg(const std::string& cmdArg, 
                                 std::vector<std::string>::iterator& parserArgCurrent, // This needs to be an iterator for this to work.
                                 std::vector<std::string>::iterator& parserArgEnd,
                                 const std::string parserArgName,
                                 const bool& parserArgRequired, 
                                 const R_xlen_t& cmdPosIndex,
                                 const R_xlen_t& cmdMaxPosition,
                                 const std::vector<R_xlen_t>& cmdArgPositions,
                                 std::list<std::vector<R_xlen_t>>& output,
                                 const std::string& helpArg,
                                 const R_xlen_t& cmdArgN){
  bool matched = false;
  while(parserArgCurrent != parserArgEnd && !matched){
    matched = cmdArg_matches_parserArg(cmdArg, *parserArgCurrent, cmdArgN);
    if(matched){
      output.push_back(get_cmdArg_range(cmdPosIndex, cmdMaxPosition, cmdArgPositions));
    }else
      parserArgCurrent++;
  }
  if(!matched && parserArgRequired)
    stop("Error: Required parser argument, \"%s\", is missing. See %s for more information about required arguments.", parserArgName, helpArg);
  return matched;
}

// Bool indicates whether the end was reached.
inline void iterateOverCmdArgs(const std::vector<std::string>& cmdArgs,
                               R_xlen_t& cmdArgCurrentIndex,
                               const R_xlen_t& cmdArgsLen,
                               const std::vector<R_xlen_t>& cmdArgPositions,
                               const R_xlen_t& cmdArgMaxPosition,
                               std::vector<std::string>::iterator& parserArgCurrent,
                               std::vector<std::string>::iterator& parserArgEnd,
                               const std::string& parserArgName,
                               const bool& parserArgRequired, // parserArgRequired is true only if it is required and has not been matched previously.
                               const std::string& helpArg,
                               std::list<std::list<std::vector<R_xlen_t>>>& output
                               ){
  bool matched = false;
  std::list<std::vector<R_xlen_t>> output_values;
  while(cmdArgCurrentIndex < cmdArgsLen){
    const R_xlen_t cmdArgCurrentPos = cmdArgPositions[cmdArgCurrentIndex];
    const std::string cmdArgCurrent = cmdArgs[cmdArgCurrentPos];
    const bool cur_matched = iterate_over_parserArg(cmdArgCurrent,
                                                     parserArgCurrent,
                                                     parserArgEnd,
                                                     parserArgName,
                                                     parserArgRequired && !matched,
                                                     cmdArgCurrentIndex,
                                                     cmdArgMaxPosition,
                                                     cmdArgPositions,
                                                     output_values,
                                                     helpArg,
                                                     cmdArgsLen);
    matched = matched || cur_matched; 
    // If we matched, we need to move on to the next parser. If we did, continue to the next cmdArgument.
    if(!cur_matched)
      break;
    cmdArgCurrentIndex++;
  }
  if(cmdArgCurrentIndex == cmdArgsLen && !matched) // bit of speed performance inefficiency here, but since it is the error message I 
    stop("Error: Unknown commandline argument \"%s\" provided. See %s for more information about available arguments.", 
         cmdArgs[cmdArgPositions[cmdArgCurrentIndex]], helpArg);
  if(matched)
    output.push_back(output_values);
}

// [[Rcpp::export(rng=false)]]
Rcpp::List match_args_c_v3(const std::vector<std::string> cmdArgs, 
                           const std::vector<R_xlen_t> cmdArgsPositionsOrdered,
                           const IntegerVector cmdArgsPositionsUnordered, // frustrating naming. Change this so that we have continuity between functions
                           const std::vector<std::vector<std::string>> parserArgs,
                           const IntegerVector parserArgsOrder,
                           const LogicalVector parserArgsRequired,
                           const std::string helpArg){
  const R_xlen_t cmdArgsLen = cmdArgsPositionsOrdered.size(),
    cmdArgsMaxPosition = *std::max_element(cmdArgsPositionsOrdered.begin(), cmdArgsPositionsOrdered.end());
  R_xlen_t cmdArgCurrentIndex = 0;

  std::list<std::list<std::vector<R_xlen_t>>> output;

  for(auto i : parserArgsOrder){
    const bool parserArgRequired = parserArgsRequired[i]; 
    std::vector<std::string> curParserArg = parserArgs[i];
    const R_xlen_t curParserArgN = curParserArg.size();
    const std::string parserArgName = curParserArg[curParserArgN - 1];
    if(cmdArgsLen == cmdArgCurrentIndex){
      if(parserArgRequired)
        stop("Error: Required parser argument, \"%s\", is missing. See %s for more information about required arguments.", parserArgName, helpArg);
      else
        continue;
    }
    
    std::vector<std::string>::iterator parserArgCurrent = curParserArg.begin(), 
            parserArgEnd = curParserArg.end();
    iterateOverCmdArgs(cmdArgs, 
                       cmdArgCurrentIndex, 
                       cmdArgsLen, 
                       cmdArgsPositionsOrdered,
                       cmdArgsMaxPosition,
                       parserArgCurrent,
                       parserArgEnd,
                       parserArgName, 
                       parserArgRequired,
                       helpArg,
                       output
  }
  return wrap(output);
}

