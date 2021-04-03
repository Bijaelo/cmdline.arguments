

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
                                            const bool& required){
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
        /* Maybe I'll enforce this, maybe not. It doesnt seem sensible to enforce right now.
        if(!is<String>(helpFlags) || as<String>(helpFlags) == NA_STRING)
          stop("unexpected error, no flags for \"help\" was found. Please report this issue to the package github with an example for reproducing this error.");
        */
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
                             required);
      }

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
        if(!is<bool>(required))
          stop("required must be either TRUE or FALSE");
        return make_info_single(name,
                                narg,
                                meta,
                                action,
                                rawPassingOption,
                                rawIngestionOption,
                                helpFlags,
                                flags,
                                choices,
                                parseFun,
                                help,
                                defaultVal,
                                constVal,
                                as<bool>(required));
      }
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
                                            const int& required){
        return make_info_single(name,
                                narg,
                                meta,
                                action,
                                rawPassingOption,
                                rawIngestionOption,
                                helpFlags,
                                flags,
                                choices,
                                parseFun,
                                help,
                                defaultVal,
                                constVal,
                                static_cast<bool>(required));
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


// Macro for generating NA string vector.
// The protect "should" be unnecessary. But rather safe than sorry.
#define repn(where, what, n) do {                              \
      PROTECT(where = Rf_allocVector(STRSXP, n));              \
      ptr = STRING_PTR(where);                                 \
      for(R_xlen_t i = 0; i < n; i++, ptr++)                   \
        *ptr = what;                                           \
      nprot++;                                                 \
} while (0)

// Small
#define nList(where, n) do{                                    \
      where = PROTECT(Rf_allocVector(VECSXP, n));              \
      nprot++;                                                 \
} while (0);                                                   \
//


      inline void assignInfo(SEXP& where, const SEXP& listInfo, R_xlen_t& j){
        if(j > 0){
          where = VECTOR_ELT(listInfo, j - 1);
        }
      }
      // Used for "in(x, table)" and "match(x, table)", to avoid constructing hash tables multiple times.
      typedef Rcpp::sugar::IndexHash<STRSXP> strHASH;
      inline vector<argument_info> make_argument_info_from_column_list(const SEXP& listInfo){
        vector<argument_info> res;
        BEGIN_RCPP
        if(TYPEOF(listInfo) != VECSXP)
          stop("Unexpected error: non-list passed to make_argument_info_from_column_list");
        SEXP names = Rf_getAttrib(listInfo, R_NamesSymbol); // Automatically protected
        // Test that we have actually received a list with column parameters.
        if(names == R_NilValue)
          stop("unnamed parameter list passed to make_argument_from_column_list");
        // Use manual hash-table
        // Avoids creating 3 hash-tables, 1 for Rcpp::duplicated, 1 for Rcpp::in, 1 for Rcpp:match
        strHASH hashTable(as<CharacterVector>(names));
        if(is_true(any(hashTable.fill_and_get_duplicated())))
          stop("duplicate parameters provided in add_argument for parameter list");
        // Use STRING_PTR as we already know the names is not NULL (eg: safe).
        SEXP* namePtr = STRING_PTR(names);
        R_xlen_t n = -1;
        for(R_xlen_t i = Rf_xlength(listInfo); i >= 0; i--) // Find name length.
          if(strcmp(CHAR(namePtr[i]), "name") == 0){
            n = Rf_xlength(VECTOR_ELT(listInfo, i));
            break;
          }
          if(n == -1)
            stop("\"name\" must be included in all calls to add_argument");
        // Create variables that needs to be passed to make_info_single
        // We'll allocate these after checking their types.
#pragma GCC diagnostic push // this should remove warnings but it doesn't
#pragma GCC diagnostic ignored "-Wall"
        SEXP name,
         narg,
         meta,
         action,
         rawPassingOption,
         rawIngestionOption,
         helpFlags,
         flags,
         choices,
         parseFun,
         help,
         defaultVal,
         constVal,
         required;
        SEXP* ptr;
#pragma GCC diagnostic pop
        // Check if we have any missing variables, that need to be filles.
        LogicalVector parametersInName(fields.begin(),
                                       fields.end(),
                                       Rcpp::sugar::InSet<strHASH>(hashTable));
        CharacterVector missing = fields[!parametersInName];
        if(n == 1){
          // We have a bit of optimization plausible when we only have a single field.
          // 1) Assignment need to be single values
          // 2) Type checking is done through make_argument_single
          // 3) We dont need to check argument length but just "assume" they're all singular
          if(missing.size() != 0){
            for(auto i: missing){
              // Could save some time by instead making it a string-view.
              string iString = as<string>(i);
              if(iString == "meta"){
                meta = NA_STRING;
              }else if(iString == "choices"){
                choices = R_NilValue;
              }else if(iString == "constVal"){
                constVal = R_NilValue;
              }else if(iString == "action"){
                repn(meta, wrap("store"), 1);
              }else if(iString == "helpFlags"){
                helpFlags = NA_STRING;
              }else if(iString == "defaultVal"){
                defaultVal = R_NilValue;
              }else if(iString == "rawPassingOption"){
                repn(rawPassingOption, wrap("combine"), 1);
              }else if(iString == "rawIngestionOption"){
                repn(rawPassingOption, wrap("vector"), 1); // default to vector
              }else if(iString == "required"){
                required = PROTECT(Rf_allocVector(LGLSXP, 1));
                nprot++;
                SET_LOGICAL_ELT(required, 0, FALSE);
              }else
                stop("Missing required parameter %s in argument list", iString);
            }
            vector<R_xlen_t> fieldsPositions(as<vector<R_xlen_t>>(hashTable.lookup(fields.get_ref())));
            R_xlen_t nF = fieldsPositions.size();
            assignInfo(name, listInfo, fieldsPositions[0]);
            assignInfo(narg, listInfo, fieldsPositions[1]);
            assignInfo(meta, listInfo, fieldsPositions[2]);
            assignInfo(action, listInfo, fieldsPositions[3]);
            assignInfo(rawPassingOption, listInfo, fieldsPositions[4]);
            assignInfo(rawIngestionOption, listInfo, fieldsPositions[5]);
            assignInfo(helpFlags, listInfo, fieldsPositions[6]);
            assignInfo(flags, listInfo, fieldsPositions[7]);
            assignInfo(choices, listInfo, fieldsPositions[8]);
            assignInfo(parseFun, listInfo, fieldsPositions[9]);
            assignInfo(help, listInfo, fieldsPositions[10]);
            assignInfo(defaultVal, listInfo, fieldsPositions[11]);
            assignInfo(constVal, listInfo, fieldsPositions[12]);
            assignInfo(required, listInfo, fieldsPositions[13]);
            res.push_back(make_info_single(name,
                                          narg,
                                          meta,
                                          action,
                                          rawPassingOption,
                                          rawIngestionOption,
                                          helpFlags,
                                          flags,
                                          choices,
                                          parseFun,
                                          help,
                                          defaultVal,
                                          constVal,
                                          required));
          }
        }else{
          // If we have multiple arguments, we need have to perform all of our type checking.
          if(missing.size() != 0){
            for(auto i: missing){
              // Could save some time by instead making it a string-view.
              string iString = as<string>(i);
              if(iString == "meta"){
                repn(meta, NA_STRING, n);
              }else if(iString == "choices"){
                nList(choices, n);
              }else if(iString == "constVal"){
                nList(constVal, n);
              }else if(iString == "action"){
                repn(meta, wrap("store"), n);
              }else if(iString == "helpFlags"){
                repn(helpFlags, NA_STRING, n);
              }else if(iString == "defaultVal"){
                nList(defaultVal, n);
              }else if(iString == "rawPassingOption"){
                repn(rawPassingOption, wrap("combine"), n);
              }else if(iString == "rawIngestionOption"){
                repn(rawPassingOption, wrap("vector"), n); // default to vector
              }else if(iString == "required"){
                required = PROTECT(Rf_allocVector(LGLSXP, n));
                nprot++;
                for(R_xlen_t i = 0; i < n; i++)
                  SET_LOGICAL_ELT(required, i, FALSE);
              }else
                stop("Missing required parameter %s in argument list", iString);
            }
          }

          // Assign the remaining parameters.
          vector<R_xlen_t> fieldsPositions(as<vector<R_xlen_t>>(hashTable.lookup(fields.get_ref())));
          R_xlen_t nF = fieldsPositions.size();
          assignInfo(name, listInfo, fieldsPositions[0]);
          assignInfo(narg, listInfo, fieldsPositions[1]);
          assignInfo(meta, listInfo, fieldsPositions[2]);
          assignInfo(action, listInfo, fieldsPositions[3]);
          assignInfo(rawPassingOption, listInfo, fieldsPositions[4]);
          assignInfo(rawIngestionOption, listInfo, fieldsPositions[5]);
          assignInfo(helpFlags, listInfo, fieldsPositions[6]);
          assignInfo(flags, listInfo, fieldsPositions[7]);
          assignInfo(choices, listInfo, fieldsPositions[8]);
          assignInfo(parseFun, listInfo, fieldsPositions[9]);
          assignInfo(help, listInfo, fieldsPositions[10]);
          assignInfo(defaultVal, listInfo, fieldsPositions[11]);
          assignInfo(constVal, listInfo, fieldsPositions[12]);
          assignInfo(required, listInfo, fieldsPositions[13]);
          // Check input types are correct.
          auto msg = "%s must be a character vector when multiple arguments are supplied to add_argument";
          if(!is<CharacterVector>(name)) // 1
            stop(msg, "name");
          if(!is<CharacterVector>(narg)) // 2
            stop(msg, "narg");
          if(!is<CharacterVector>(meta)) // 3
            stop(msg, "meta");
          if(!is<CharacterVector>(action)) // 4
            stop(msg, "action");
          if(!is<CharacterVector>(rawPassingOption))
            stop(msg, "rawPassingOption");
          if(!is<CharacterVector>(rawIngestionOption))
            stop(msg, "rawIngestionOption");
          if(!is<CharacterVector>(helpFlags))
            stop(msg, "helpFlags");
          if(!is<CharacterVector>(flags))
            stop(msg, "flags");
          msg = "%s must be a list when multiple arguments are supplied to add_argument";
          if(!is<List>(choices))
            stop(msg, "choices");
          if(!is<List>(constVal))
            stop(msg, "const");
          if(!is<List>(parseFun))
            stop(msg, "parseFun");
          if(!is<List>(help))
            stop(msg, "help");
          if(!is<List>(defaultVal))
            stop(msg, "default");
          if(!is<LogicalVector>(required))
            stop("%s must be a logical vector when multiple arguments are supplied to add_argument", "required");
          if(!ALTREP(name) &&
             !ALTREP(narg) &&
             !ALTREP(narg) &&
             !ALTREP(meta) &&
             !ALTREP(action) &&
             !ALTREP(rawPassingOption) &&
             !ALTREP(rawIngestionOption) &&
             !ALTREP(helpFlags) &&
             !ALTREP(flags)
          ){
            // If we are not working with ALTREP's we can abuse DATAPTR
            SEXP *nameptr = STRING_PTR(name),
              *nargptr = STRING_PTR(narg),
              *metaptr = STRING_PTR(meta),
              *actionptr = STRING_PTR(action),
              *rawPassingOptionptr = STRING_PTR(rawPassingOption),
              *rawIngestionOptionptr = STRING_PTR(rawIngestionOption),
              *helpflagsptr = STRING_PTR(helpFlags),
              *flagsptr = STRING_PTR(flags);
              for(R_xlen_t i = 0; i < n;
              i++,
              nameptr++,
              nargptr++,
              metaptr++,
              actionptr++,
              rawPassingOptionptr++,
              rawIngestionOptionptr++,
              helpflagsptr++,
              flagsptr++){
                res.push_back(make_info_single( *nameptr,
                                                *nargptr,
                                                *metaptr,
                                                *actionptr,
                                                *rawPassingOptionptr,
                                                *rawIngestionOptionptr,
                                                *helpflagsptr,
                                                *flagsptr,
                                                VECTOR_ELT(choices, i),
                                                VECTOR_ELT(parseFun, i),
                                                VECTOR_ELT(help, i),
                                                VECTOR_ELT(defaultVal, i),
                                                VECTOR_ELT(constVal, i),
                                                LOGICAL_ELT(required, i))); // <== LOGICAL_ELT returns int and not SEXP
              }
          }else{
            for(R_xlen_t i = 0; i < n; i++){
              res.push_back(make_info_single( STRING_ELT(name, i),
                                              STRING_ELT(narg, i),
                                              STRING_ELT(meta, i),
                                              STRING_ELT(action, i),
                                              STRING_ELT(rawPassingOption, i),
                                              STRING_ELT(rawIngestionOption, i),
                                              STRING_ELT(helpFlags, i),
                                              STRING_ELT(flags, i),
                                              VECTOR_ELT(choices, i),
                                              VECTOR_ELT(parseFun, i),
                                              VECTOR_ELT(help, i),
                                              VECTOR_ELT(defaultVal, i),
                                              VECTOR_ELT(constVal, i),
                                              LOGICAL_ELT(required, i))); // <== LOGICAL_ELT returns int and not SEXP
            }
          }
        }
        VOID_END_RCPP
        return res;
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
