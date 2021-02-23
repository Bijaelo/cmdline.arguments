


#ifndef cmdline_pfunc
#define cmdline_pfunc


#include "utils/traits/traits.h"
#include "RcppApi/ArgumentList.h"
//#include "utils/converters/converters.h"
using std::string;
using Rcpp::List, Rcpp::stop, Rcpp::Function, Rcpp::ArgumentList;
using namespace cmdline_arguments::traits; // for  types.

using cci = CharacterVector::const_iterator;
using aci = ArgumentList::const_iterator;
/* Parser function implementation
 *
 * Implemented as a thin layer on top of the Rcpp::Function implementation.
 * Main differences:
 * Constructor allows for 2 additional parameters: name (string) and args
 * (SEXP/List), pass_as_name (string).
 * parserFunction requires a typename specification (a trait).
 * This mainly handles how arguments are passed to func. This can be one of:
 * - individual_input: arguments pass via operator(std::vector<std::string>)
 *   are passed as individual arguments from position 1 to k.f
 * - vector_input: arguments are passed as a CharacterVector
 * - list_input: arguments are passed as a list of individual components in position K.
 * pass_as_name (optional) allows for optional naming of argument passed via
 *   operator(). This is currently only viable option for specifying argument
 *   position outside of naming all other arguments passed in args.
 */

namespace cmdline_arguments::parser{


  // The following 3 implementation have the same constructors (shame on me)
  // but the actual method needs to be different
  class parserFunction{
  public:
    // Need constructors for
    // 1: function, name
    // 2: function, args, name

    // function, Name
    template<typename T, typename S>
    parserFunction(T fun, S _name):
      name(_name), func(fun), args(List::create()){ }
    // function, name, arguments
    template<typename T, typename S, typename U>
    parserFunction(T fun, S _name, U _args):
      name(_name), func(fun), args(_args){ }



    template<typename T>
    SEXP operator()(const T& rawArgs){
      const List rawArgsList(rawArgs);
      R_xlen_t rawSize = rawArgsList.size(),
        argSize = (this -> args).size(),
        tot = rawSize + argSize;
      if(rawSize > 0){
        // we have at least 1 argument to pass
        List combined(tot);
        CharacterVector names(tot),
          rawNames(rawArgs.names()),
          args_names((this -> args).names());
        List::iterator combined_Iterator = combined.begin();
        CharacterVector::iterator names_Iterator = names.begin();

        List::const_iterator rawBegin = rawArgsList.begin(), rawEnd = rawArgsList.end();
        AppendArguments(combined_Iterator, names_Iterator,
                   rawBegin, rawEnd,
                   rawNames.cbegin(), rawNames.size());

        List::const_iterator argsBegin = (Rcpp::as<List>(this -> args)).begin(),
              argsEnd = (Rcpp::as<list>(this -> args)).end();
        AppendArguments(combined_Iterator, names_Iterator,
                   argsBegin, argsEnd,
                   args_names.cbegin(), args_names.size());
        /*
        // Fill in name and the combine ArguentList
        // We start with the rawArgs, as they should come first.
        List::const_iterator
          Start = rawArgsList.cbegin(), End = rawArgsList.cend(),
            combined_Iterator = combined.cbegin();

        CharacterVector Names = rawArgsList.names();
        CharacterVector::const_iterator names_Iterator = names.cbegin(),
          NStart = Names.cbegin();
        R_xlen_t ni = Names.size();
        for(; Start != End; Start++, combined_Iterator++){
          *combined_Iterator = *Start;
          // append names if present
          if(ni > 0){
            if((*NStart) != R_NilValue && CHAR((*NStart))[0] != '\0')
              *names_Iterator = *NStart;
            NStart++;
          }
        }
        // then append the remaining args.
        Names = (this -> args).names();
        ni = Names.size();
        //NStart = Names.cbegin();
        //Start = (this -> args).cbegin();
        //End = (this -> args).cend();
        // Really should place this into a separate function.
        // Maybe after I'm sure it works as expected
        for(; Start != End; Start++, combined_Iterator++){
          *combined_Iterator = *Start;
          // append names if present
          if(ni > 0){
            if((*NStart) != R_NilValue && CHAR((*NStart))[0] != '\0')
              *names_Iterator = *NStart;
            NStart++;
          }
        }
        */
        combined.attr("names") = names;
        return (this -> func)(Rcpp::as<ArgumentList>(combined));
      }else // No args provided, just execute the f unction.
        if(argSize > 0) return (this -> func)(this -> args);
        else return(this -> func)();
    }

  private:
    const string name;
    const Function func;
    const List args;
    void const AppendArguments(List::iterator& outputArgs,
                                CharacterVector::iterator& outputNames,
                                List::const_iterator& Start,
                                List::const_iterator& End,
                                CharacterVector::const_iterator& names,
                                R_xlen_t namesLength){
      // Implement the for loop above, as to avoid "other.proxy" error occuring.
      for(; Start != End; Start++, outputArgs++, outputNames++){
        *outputArgs = *Start;
        if(namesLength > 0){
          // This fucks up a bit.
          if((*names) != "" && (*names) != nullptr){
            *outputNames = *names;
          }
          names++;
        }
      }
    }

  };


 /* Thought 2021-02-17:
  *
  * Let individual_input, list_input and vector_input store input
  * and handle how to extract input.
  *
  * So for example:
  * Vector_inputs has a field called "data".  this field is a raw string.
  * It also has a getter "getData". data is set using = or (), and can be
  * extended using += or "x.data = x.data + string"
  * getData however returns data as either a List or CharacterVector
  * ready to be added to pairlist. Or maybe even pairlist element itself.
  *
  * At this point it becomes extremely simple to actually handle calling the
  * function.
  * We simple call parserFunction x; x(args.getdata());
  * And that should simply return data.
  *
  *
  * We still have probles with how we'll handle "store_const" (if this feature
  * makes it into the final version) but that is a problem for another day.
  * This basically will make do_docall much **much** simpler to understand.
  *
  */
}


#endif
