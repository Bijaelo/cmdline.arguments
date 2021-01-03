

#include <utils/matching.h>


namespace cmdline_arguments{
  namespace utils{
    namespace matching{
/*
 * find_args finds the position of arguments given long and short args
 * reports an error for inconsistent arguments, and -1 if help was requested.
 * Otherwise an integer vector is returned, with "argLen" attribute indicating if it was a long or short arg.
 *
 */
      IntegerVector find_args_c(const std::vector<std::string>& x,
                                const std::string& argS,
                                const std::string& argL){
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
                Rcpp::stop("unexpected space \" \" found after argument indicator at position %i", i + 1);
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
                Rcpp::stop("unexpected space \" \" found after argument indicator at position %i", i + 1);
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

      IntegerVector find_args_single_c(const std::vector<std::string>& x,
                                       const std::string& arg){
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
                Rcpp::stop("unexpected space \" \" found after argument indicator at position %i", i + 1);
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


      // Faster substring function for our specific needs
      CharacterVector substring_c(const std::vector<std::string>& x,
                                  const std::vector<long>& l){
        const R_xlen_t j = x.size();
        std::vector<std::string> o;
        o.reserve(j);
        for(R_xlen_t i = 0; i < j; i++){
          const std::string xi = x[i];
          o.push_back(xi.substr(l[i], xi.size()));
        }
        return wrap(o);
      }


      /* Perform a single match for a commandArg against a parserArg.
       * Use referenced objects to avoid unnecessary copying.
       */

      inline bool cmdArg_matches_parser_arg(const std::string& cmdArg,
                                           const std::string& parserArg,
                                           const R_xlen_t& cmdArgN){
        const int cmn = cmdArg.size(), pan = parserArg.size();
        return (((cmn == pan) && (cmdArg == parserArg)) ||
                ((cmn > pan) && (cmdArg.substr(0, pan) == parserArg) && (cmdArg.at(pan) == '=')));
      }


      // Generate a vector of indices over a specific argument
      inline std::vector<R_xlen_t> get_cmd_arg_range(const R_xlen_t& cmdPosIndex,
                                                    const R_xlen_t& cmdMaxPosition,
                                                    const std::vector<R_xlen_t>& cmdArgPositions,
                                                    const std::vector<R_xlen_t>& cmdArgsNextPositions,
                                                    const R_xlen_t& cmdArgN){
        // This may have to be done in 2 lines:
        return {cmdArgPositions[cmdPosIndex],
                (cmdPosIndex == cmdMaxPosition) ? cmdArgN : cmdArgsNextPositions[cmdPosIndex]};
      }

      /* Iterate over the parserArgs for a given cmdArg. Return a boolean indicating the outcome
       * false: Reached end of parserArg and did not find a match
       * true: Value was matched and appended
       */
      inline bool iterate_over_parser_arg(const std::string& cmdArg,
                                       std::vector<std::string>::iterator& parserArgCurrent, // This needs to be an iterator for this to work.
                                       std::vector<std::string>::iterator& parserArgEnd,
                                       const std::string parserArgName,
                                       const bool& parserArgRequired,
                                       const R_xlen_t& cmdPosIndex,
                                       const R_xlen_t& cmdMaxPosition,
                                       const std::vector<R_xlen_t>& cmdArgPositions,
                                       const std::vector<R_xlen_t>& cmdArgsNextPositions,
                                       std::list<std::vector<R_xlen_t>>& output,
                                       const std::string& helpArg,
                                       const R_xlen_t& cmdArgN){
        bool matched = false;
        while(parserArgCurrent != parserArgEnd && !matched){
          matched = cmdArg_matches_parser_arg(cmdArg, *parserArgCurrent, cmdArgN);
          if(matched){
            output.push_back(get_cmd_arg_range(cmdPosIndex, cmdMaxPosition, cmdArgPositions, cmdArgsNextPositions, cmdArgN));
          }else
            parserArgCurrent++;
        }
        if(!matched && parserArgRequired)
          stop("required parser argument, '%s', is missing. See '%s' for more information about required arguments", parserArgName, helpArg);
        return matched;
      }


      inline void iterate_over_cmd_args(const std::vector<std::string>& cmdArgs,
                                     R_xlen_t& cmdArgCurrentIndex,
                                     const R_xlen_t& cmdArgsLen,
                                     const std::vector<R_xlen_t>& cmdArgPositions,
                                     const std::vector<R_xlen_t>& cmdArgsNextPositions,
                                     const R_xlen_t& cmdArgMaxPosition,
                                     std::vector<std::string>::iterator& parserArgCurrent,
                                     std::vector<std::string>::iterator& parserArgEnd,
                                     const std::string& parserArgName,
                                     const bool& parserArgRequired, // parserArgRequired is true only if it is required and has not been matched previously.
                                     const std::string& helpArg,
                                     std::list<std::list<std::vector<R_xlen_t>>>& output,
                                     std::vector<std::string>& names
                                     ){
        bool matched = false;
        std::list<std::vector<R_xlen_t>> output_values;
        while(cmdArgCurrentIndex < cmdArgsLen){
          const R_xlen_t cmdArgCurrentPos = cmdArgPositions[cmdArgCurrentIndex];
          const std::string cmdArgCurrent = cmdArgs[cmdArgCurrentPos];
          const bool cur_matched = iterate_over_parser_arg(cmdArgCurrent,
                                                           parserArgCurrent,
                                                           parserArgEnd,
                                                           parserArgName,
                                                           parserArgRequired && !matched,
                                                           cmdArgCurrentIndex,
                                                           cmdArgMaxPosition,
                                                           cmdArgPositions,
                                                           cmdArgsNextPositions,
                                                           output_values,
                                                           helpArg,
                                                           cmdArgsLen);
          if(cur_matched && !matched)
            names.push_back(parserArgName);
          matched = matched || cur_matched;
          // If we matched, we need to move on to the next parser. If we did, continue to the next cmdArgument.
          if(!cur_matched)
            break;
          cmdArgCurrentIndex++;
        }
        if(cmdArgCurrentIndex == cmdArgsLen && !matched) // bit of speed performance inefficiency here, but since it is the error message I
          stop("unknown commandline argument \'%s\' provided. See '%s' for more information about available arguments",
               cmdArgs[cmdArgPositions[cmdArgCurrentIndex]], helpArg);
        if(matched)
          output.push_back(output_values);
      }

      Rcpp::List extract_args(const std::vector<std::string>& cmdArgs,
                              const std::list<std::list<std::vector<R_xlen_t>>>& match_indices,
                              const std::vector<std::string>& names){
        std::list<std::list<std::vector<std::string>>> output;
        auto b = match_indices.begin(), e = match_indices.end();
        // Iterate over the out list (individual parserArgs)
        for(; b != e; b++){
          std::list<std::vector<std::string>> oi;
          std::list<std::vector<R_xlen_t>>::const_iterator bi = (*b).begin(), ei = (*b).end();
          // Iterate over the inner list (cmd arguments distributed to specific parserArgs arguments)
          for(; bi != ei; bi++){
            std::vector<R_xlen_t> bii = (*bi);
            std::vector<std::string> ois;
            const R_xlen_t f = bii[0], l = bii[1];
            ois.reserve((long int)(l - f + 1));
            auto iie = cmdArgs.begin() + l;
            // Add the elements of our vector to our output
            for(auto ii = cmdArgs.begin() + f; ii <= iie; ii++){
              ois.push_back((*ii));
            }
            oi.push_back(ois);
          }
          output.push_back(oi);
        }
        Rcpp::List o = wrap(output);
        o.attr("names") = names;
        return o;
      }

      Rcpp::List match_args_c(const std::vector<std::string>& cmdArgs,
                              const std::vector<R_xlen_t>& cmdArgsPositions,
                              const std::vector<R_xlen_t>& cmdArgsNextPositions, // frustrating naming. Change this so that we have continuity between functions
                              const std::vector<std::vector<std::string>>& parserArgs,
                              const IntegerVector& parserArgsOrder,
                              const LogicalVector& parserArgsRequired,
                              const std::string& helpArg){
        const R_xlen_t cmdArgsLen = cmdArgsPositions.size(),
          cmdArgsMaxPosition = *std::max_element(cmdArgsPositions.begin(), cmdArgsPositions.end());
        R_xlen_t cmdArgCurrentIndex = 0;
        std::list<std::list<std::vector<R_xlen_t>>> output;
        std::vector<std::string> names;
        for(auto i : parserArgsOrder){
          const bool parserArgRequired = parserArgsRequired[i];
          std::vector<std::string> curParserArg = parserArgs[i];
          const R_xlen_t curParserArgN = curParserArg.size();
          const std::string parserArgName = curParserArg[curParserArgN - 1];
          if(cmdArgsLen == cmdArgCurrentIndex){
            if(parserArgRequired)
              stop("required parser argument, '%s', is missing. See '%s' for more information about required arguments", parserArgName, helpArg);
            else
              continue;
          }
          std::vector<std::string>::iterator parserArgCurrent = curParserArg.begin(),
                  parserArgEnd = curParserArg.end();
          iterate_over_cmd_args(cmdArgs,
                                cmdArgCurrentIndex,
                                cmdArgsLen,
                                cmdArgsPositions,
                                cmdArgsNextPositions,
                                cmdArgsMaxPosition,
                                parserArgCurrent,
                                parserArgEnd,
                                parserArgName,
                                parserArgRequired,
                                helpArg,
                                output,
                                names);
        }
        //Rcpp::List o = wrap(output);
        //o.attr("names") = names;
        return extract_args(cmdArgs, output, names);
        //return extract_args(cmdArgs, output, names);
      }

    }
  }
}
