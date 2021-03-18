

// Container for holding information.

#ifndef cmd_args_argument_info_h
#define cmd_args_argument_info_h

#include <string>
#include <vector>
#include <Rcpp.h>
#include <algorithm>
#include <cctype>
#include <map>
#include <utility>
#include "utils/sugar.h"
using namespace std;
using namespace Rcpp;
using namespace cmd_args::utils::sugar;

namespace cmd_args{
  namespace parser{
    namespace argument {
      /* c-style struct.
       *
       * Simple container for holding shared information across multiple components.
       *
       * Many components of the argument exist to "encapsulate" a certain part of
       * the cmdline argument reader. Such as "raw_argument", "raw_Container" and
       * "parserFunction". However most of these require some or all of the same
       * arguments,
       */
      struct argument_info {

        string name,
          narg,
          meta,
          action,
          rawPassingOption,
          rawIngestionOption,
          helpFlags;

        vector<string> flags,
          choices;

        RObject parseFun,
          help,
          defaultVal,
          constVal;

        bool required;
        argument_info(){};
        argument_info(const argument_info& x):
          name(x.name),
          meta(x.meta),
          action(x.action),
          rawPassingOption(x.rawPassingOption),
          rawIngestionOption(x.rawIngestionOption),
          helpFlags(x.helpFlags),
          flags(x.flags),
          choices(x.choices),
          parseFun(x.parseFun),
          help(x.help),
          defaultVal(x.defaultVal),
          constVal(x.constVal),
          required(x.required)
          {}
        argument_info(string NAME,
                      string NARG,
                      string META,
                      string ACTION,
                      string RAWPASSINGOPTION,
                      string RAWINGESTIONOPTION,
                      string HELPFLAGS,
                      vector<string> FLAGS,
                      vector<string> CHOICES,
                      RObject PARSEFUN,
                      RObject HELP,
                      RObject DEFAULTVAL,
                      RObject CONSTVAL,
                      bool REQUIRED):
          name(NAME),
          narg(NARG),
          meta(META),
          action(ACTION),
          rawPassingOption(RAWPASSINGOPTION),
          rawIngestionOption(RAWINGESTIONOPTION),
          helpFlags(HELPFLAGS),
          flags(FLAGS),
          choices(CHOICES),
          parseFun(PARSEFUN),
          help(HELP),
          defaultVal(DEFAULTVAL),
          constVal(CONSTVAL),
          required(REQUIRED)
        {}
      };

      // Rcpp version of anyNA (does not seem to exist in Rcpp) (removed as it is slower than calling R's internal function.)
/*      template<typename T, typename S>
      bool any_na(S x){
        T xx = as<T>(x);
        for(auto i: xx){
          if(T::is_na(i))
            return true;
        }
        return false;
      }*/
      /* Wrappers for creating argument_info and checking input type.
       *
       * Create a single instance of "argument_info" checking that each element
       * is provided correctly. We "assume" that every element has length 1.
       *
       * This is only placed inside C++ to reduce iteration time in R.
       * Some parts are actually slower than they were in their R equivalent (namely anyNA is slower in Rcpp??)
       *
       */
      inline argument_info make_info_single(const SEXP& name,
                                            const SEXP& narg,
                                            const SEXP& meta,
                                            const SEXP& action,
                                            const SEXP& rawPassingOption,
                                            const SEXP& rawIngestionOption,
                                            const SEXP& helpFlags,
                                            const SEXP& flags,
                                            const SEXP& choices,
                                            const SEXP& parseFun,
                                            const SEXP& help,
                                            const SEXP& defaultVal,
                                            const SEXP& constVal,
                                            const SEXP& required){
        if(!is<String>(name) || as<String>(name) == NA_STRING)
          stop("argument name must be a single string");
        string stringName = as<string>(name);
        if(!is<String>(narg) || as<String>(narg) == NA_STRING)
          stop("narg must be one of [%s] where N is a positive integer.",
               "\"?\", \"-\", \"+\", \"*\", \"N\", \"N-\", \"N+\"");
        string stringNarg = as<string>(narg),
          stringMeta;
        if(!is<String>(meta) || as<String>(meta) == NA_STRING){
          // Default meta to capitalized name
          stringMeta = stringName;
          transform(stringMeta.begin(), stringMeta.end(), stringMeta.begin(), [](unsigned char c){return toupper(c);});
        }else
          stringMeta = as<string>(meta);
        if(!is<String>(action) || as<String>(action) == NA_STRING)
          stop("action must be one of (%s).",
               "\"store[_value]\", \"const\", \"store_true\", \"store_false\"");
        string stringAction = as<string>(action);
        if(!is<String>(rawPassingOption) || as<String>(rawPassingOption) == NA_STRING)
          stop("Undefined input type provided [Given=%s, valid={%s}].",
                     "Unknown-non-string-type",
                     "individual[_type], vector[_type], list[_type]");
        string stringRawPassingOption = as<string>(rawPassingOption);
        if(!is<String>(rawIngestionOption) || as<String>(rawIngestionOption) == NA_STRING)
          stop("Undefined option specified for handling of flags provided multiple times [Given=%s, valid={%s}]",
                     "Unknown-non-string-type",
                     "individual, vector, combine");
        string stringRawIngestionOption = as<string>(rawIngestionOption);
        if(!is<String>(helpFlags) || as<String>(helpFlags) == NA_STRING)
          stop("unexpected error, no flags for \"help\" was found. Please report this issue to the package github with an example for reproducing this error.");
        string stringHelpFlags = as<string>(helpFlags);
        vector<string> vectorFlags;
        Function any_na("anyNA"); // Faster than the Rcpp implementation. Not exactly sure why.
        if(!is<CharacterVector>(flags) || as<bool>(any_na(flags))){
          if(!is<String>(flags) || as<String>(flags) == NA_STRING)
            stop("\"flags\" must be a Character vector.");
          vectorFlags = {as<string>(flags)};
        }else
          vectorFlags = as<vector<string>>(flags);
        vector<string> vectorChoices;
        if(is<CharacterVector>(choices) && !as<bool>(any_na(choices)))
          vectorChoices = as<vector<string>>(choices);
        else if(!Rf_isNull(choices))
          stop("Currently choices must be a character vector or NULL. This may change in the future when a reasonable implementation has been thought of. Contributions are welcome.");
        if((!is<String>(parseFun) || as<String>(parseFun) == NA_STRING) && !is<Function>(parseFun))
          stop("parseFun must be either a single string or a callable function.");
        RObject RObjectParseFun = as<RObject>(parseFun);
        if((!is<String>(help) || as<String>(help) == NA_STRING) && !is<Function>(help))
          stop("help must either be a string message or a callable function.");
        // defaultVal and constVal can be anything so no checkin
        RObject RObjectHelp = as<RObject>(help),
          RObjectDefaultVal = as<RObject>(defaultVal),
          RObjectConstVal = as<RObject>(constVal);
        if(!is<bool>(required))
          stop("required must be either TRUE or FALSE");
        bool boolRequired = as<bool>(required);
        return argument_info(stringName,
                           stringNarg,
                           stringMeta,
                           stringAction,
                           stringRawPassingOption,
                           stringRawIngestionOption,
                           stringHelpFlags,
                           vectorFlags,
                           vectorChoices,
                           RObjectParseFun,
                           RObjectHelp,
                           RObjectDefaultVal,
                           RObjectConstVal,
                           boolRequired);
      }

      static const CharacterVector fields = {"name",
                                             "narg",
                                             "meta",
                                             "action",
                                             "rawPassingOption",
                                             "rawIngestionOption",
                                             "helpFlags",
                                             "flags",
                                             "choices",
                                             "parseFun",
                                             "help",
                                             "defaultVal",
                                             "constVal",
                                             "required"};

      // Used for "in(x, table)" and "match(x, table)", to avoid constructing hash tables multiple times.
      typedef Rcpp::sugar::IndexHash<STRSXP> strHASH;

      /* This function needs to be smarter.
       * Currently converts everything to list, which causes make_info_single to fail.
       * The proper way to do this would be to perform the checks from make_info_single
       * and then iterate over each element in the appropriate fashion.
       *
       * There might be an easy way around it, but I don't think this makes sense.
       *
       * So basically: Rewrite the function from scratch.
       * Go the long and bothersome way of checking every element in listInfo.
       *
       * After checking the type for each vector/list in listInfo, we avoid the
       * need to check the type of each element (and for NULL's in many cases).
       * But it does require some extra boiler plate code.
       *
       * BLAAARGH
       */
      inline vector<argument_info> make_argument_info_from_column_list(const SEXP& listInfo){
        // The list contains multiple inputs.
        if(TYPEOF(listInfo) != VECSXP)
          stop("Unexpected error: non-list passed to make_argument_info_from_column_list");
        SEXP names = Rf_getAttrib(listInfo, R_NamesSymbol); // Automatically protected
        // Test that we have actually received a list with column parameters.
        if(names == R_NilValue)
          stop("unnamed parameter list passed to make_argument_from_column_list");
        if(anyDuplicated(names))
          stop("duplicate parameters provided in add_argument for parameter list");
        // same length check does not work because of "flags", "choices" and "constVal".
        // Need to rethink this check.
        //if(!same_lengths(listInfo))
        //  stop("all parameters provided must have the same length");
        R_xlen_t n = Rf_xlength(VECTOR_ELT(listInfo, 0));

        // Manually evaluate "in" in steps, as we potentially both need a call to "in" and "match".
        // This removes the need to create hashTable twice.
        strHASH hashTable(as<CharacterVector>(names));
        hashTable.fill();
        LogicalVector fieldsInNames(fields.begin(),
                                    fields.end(),
                                    Rcpp::sugar::InSet<strHASH>(hashTable));
        CharacterVector missing = fields[!fieldsInNames];
        { // get "&name" etc out of scope here.
          if(missing.size() > 0){
            map<string, List> args;
            for(auto i : missing){
              string ii = as<string>(i);
              if(ii == "choices" || ii == "constVal" || ii == "defaultVal" || ii == "meta"){
                List appends(n);
                args[ii] = appends;
              }else
                stop("Missing required parameter %s in argument list", ii);
            }
            // Add the existing arguments.
            SEXP* ptr = STRING_PTR(names);
            for(R_xlen_t j = 0; j < n; j++){
              args[CHAR(ptr[j])] = VECTOR_ELT(listInfo, j);
            }
            List &name = args["name"],
                 &narg = args["narg"],
                 &meta = args["meta"],
                 &action = args["action"],
                 &rawPassingOption = args["rawPassingOption"],
                 &rawIngestionOption = args["rawIngestionOption"],
                 &helpFlags = args["helpFlags"],
                 &choices = args["choices"],
                 &parseFun = args["parseFun"],
                 &help = args["help"],
                 &defaultVal = args["defaultVal"],
                 &constVal = args["constVal"],
                 &required = args["required"],
                 &flags = args["flags"];

            // Does not work: Needs to subset args too.

            vector<argument_info> out;
            for(R_xlen_t i = 0; i < n; n ++){
              out.push_back(make_info_single(name[i],
                                             narg[i],
                                             meta[i],
                                             action[i],
                                             rawPassingOption[i],
                                             rawIngestionOption[i],
                                             helpFlags[i],
                                             flags[i],
                                             choices[i],
                                             parseFun[i],
                                             help[i],
                                             defaultVal[i],
                                             constVal[i],
                                             required[i]
                                             ));
            }
            return out;

          }
        }

        // No missing values. Match fields to names in listInfo and iterate over these.
        vector<R_xlen_t> fieldsPositions(as<vector<R_xlen_t>>(hashTable.lookup(fields.get_ref())));
        // Convert everything to "List" (should be constant time complexity).
        // This allows standards subsetting for all arguments rather than using
        // STRING_ELT, INTEGER_ELT, REAL_ELT etc.
        List name = VECTOR_ELT(listInfo, fieldsPositions[0]),
             narg = VECTOR_ELT(listInfo, fieldsPositions[1]),
             meta = VECTOR_ELT(listInfo, fieldsPositions[2]),
             action = VECTOR_ELT(listInfo, fieldsPositions[3]),
             rawPassingOption = VECTOR_ELT(listInfo, fieldsPositions[4]),
             rawIngestionOption = VECTOR_ELT(listInfo, fieldsPositions[5]),
             helpFlags = VECTOR_ELT(listInfo, fieldsPositions[6]),
             flags = VECTOR_ELT(listInfo, fieldsPositions[7]),
             choices = VECTOR_ELT(listInfo, fieldsPositions[8]),
             parseFun = VECTOR_ELT(listInfo, fieldsPositions[9]),
             help = VECTOR_ELT(listInfo, fieldsPositions[10]),
             defaultVal = VECTOR_ELT(listInfo, fieldsPositions[11]),
             constVal = VECTOR_ELT(listInfo, fieldsPositions[12]),
             required = VECTOR_ELT(listInfo, fieldsPositions[13]);
        vector<argument_info> out;

        for(R_xlen_t i = 0; i < n; n++){
          out.push_back(make_info_single(name[i],
                                         narg[i],
                                         meta[i],
                                         action[i],
                                         rawPassingOption[i],
                                         rawIngestionOption[i],
                                         helpFlags[i],
                                         flags[i],
                                         choices[i],
                                         parseFun[i],
                                         help[i],
                                         defaultVal[i],
                                         constVal[i],
                                         required[i]
          ));
        }
        return out;
      }

      inline vector<argument_info> make_argument_info_from_row_list(const SEXP& listInfo){
        // each entry in the list contains
        if(TYPEOF(listInfo) != VECSXP)
          stop("Unexpected error: non-list passed to make_argument_info_from_row_list");
        SEXP names = Rf_getAttrib(listInfo, R_NamesSymbol);
        if(!Rf_isNull(names))
          return make_argument_info_from_column_list(listInfo);
        vector<argument_info> intermediary, out;
        auto back = back_inserter(out);
        R_xlen_t n = Rf_xlength(listInfo);
        // instead of complicating things, we'll
        // just be a bit inefficient and call *_from_column_list
        // instead and "move" the result into an output vector.
        // This takes care of all the error checking for us,
        // and makes life simple (at only small costs).
        for(R_xlen_t i = 0; i < n; i++){
          intermediary = make_argument_info_from_column_list(VECTOR_ELT(listInfo, i));
          move(intermediary.begin(), intermediary.end(), back);
          intermediary.clear(); // make sure it is cleared for the next time. (likely unnecessary).
        }
        return out;
      }
    } // ~arguments
  } //  ~parser
} // ~cmd_args

#endif
