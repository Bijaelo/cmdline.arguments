
#ifndef _cmdline_matching_h
#define _cmdline_matching_h

#include <Rcpp.h>
#include <vector>
#include <string>
using namespace Rcpp;

namespace cmdline_arguments{
  namespace utils{
    namespace matching{
      IntegerVector find_args_c(const std::vector<std::string>& x,
                                const std::string& argS,
                                const std::string& argL);

      IntegerVector find_args_single_c(const std::vector<std::string>& x,
                                       const std::string& arg);

      CharacterVector substring_c(const std::vector<std::string>& x,
                                  const std::vector<long>& l);

      inline bool cmdArg_matches_parser_arg(const std::string& cmdArg,
                                           const std::string& parserArg,
                                           const R_xlen_t& cmdArgN);

      inline std::vector<R_xlen_t> get_cmd_arg_range(const R_xlen_t& cmdPosIndex,
                                                     const R_xlen_t& cmdMaxPosition,
                                                     const std::vector<R_xlen_t>& cmdArgPositions,
                                                     const std::vector<R_xlen_t>& cmdArgsNextPositions,
                                                     const R_xlen_t& cmdArgN);

      inline bool iterate_over_parser_arg(const std::string& cmdArg,
                                          std::vector<std::string>::iterator& parserArgCurrent,
                                          std::vector<std::string>::iterator& parserArgEnd,
                                          const std::string parserArgName,
                                          const bool& parserArgRequired,
                                          const R_xlen_t& cmdPosIndex,
                                          const R_xlen_t& cmdMaxPosition,
                                          const std::vector<R_xlen_t>& cmdArgPositions,
                                          const std::vector<R_xlen_t>& cmdArgsNextPositions,
                                          std::list<std::vector<R_xlen_t>>& output,
                                          const std::string& helpArg,
                                          const R_xlen_t& cmdArgN);


      inline void iterate_over_cmd_args(const std::vector<std::string>& cmdArgs,
                                        R_xlen_t& cmdArgCurrentIndex,
                                        const R_xlen_t& cmdArgsLen,
                                        const std::vector<R_xlen_t>& cmdArgPositions,
                                        const std::vector<R_xlen_t>& cmdArgsNextPositions,
                                        const R_xlen_t& cmdArgMaxPosition,
                                        std::vector<std::string>::iterator& parserArgCurrent,
                                        std::vector<std::string>::iterator& parserArgEnd,
                                        const std::string& parserArgName,
                                        const bool& parserArgRequired,
                                        const std::string& helpArg,
                                        std::list<std::list<std::vector<R_xlen_t>>>& output,
                                        std::vector<std::string>& names);

      Rcpp::List extract_args(const std::vector<std::string>& cmdArgs,
                              const std::list<std::list<std::vector<R_xlen_t>>>& match_indices,
                              const std::vector<std::string>& names);

      Rcpp::List match_args_c(const std::vector<std::string>& cmdArgs,
                              const std::vector<R_xlen_t>& cmdArgsPositions,
                              const std::vector<R_xlen_t>& cmdArgsNextPositions, // frustrating naming. Change this so that we have continuity between functions
                              const std::vector<std::vector<std::string>>& parserArgs,
                              const IntegerVector& parserArgsOrder,
                              const LogicalVector& parserArgsRequired,
                              const std::string& helpArg);
    }
  }
}

#endif
