


#ifndef cmdline_pfunc
#define cmdline_pfunc


#include "utils/traits/traits.h"
//#include "utils/converters/converters.h"
using std::string;
using Rcpp::List, Rcpp::stop, Rcpp::Shield, Rcpp::Function;
using namespace cmdline_arguments::traits;
/* Parser function implementation
 *
 * Implemented as a thin layer on top of the Rcpp::Function implementation.
 * Main differences:
 * Constructor allows for 2 additional parameters: name (string) and args
 * (SEXP/List), pass_as_name (string).
 * parser_Function requires a typename specification (a trait).
 * This mainly handles how arguments are passed to func. This can be one of:
 * - individual_input: arguments pass via operator(std::vector<std::string>)
 *   are passed as individual arguments from position 1 to k.
 * - vector_input: arguments are passed as a CharacterVector
 * - list_input: arguments are passed as a list of individual components in position K.
 * pass_as_name (optional) allows for optional naming of argument passed via
 *   operator(). This is currently only viable option for specifying argument
 *   position outside of naming all other arguments passed in args.
 */

namespace cmdline_arguments::parser{


  // The following 3 implementation have the same constructors (shame on me)
  // but the actual method needs to be different

  class parser_Function{
  public:
    parser_Function(string, string);
    parser_Function(string, string, SEXP);
    parser_Function(string, string, List);
    parser_Function(string, string, SEXP, string);
    parser_Function(string, string, List, string);

    template<typename T>
    bool operator()();

  private:
    string name;
    Function func;
    Shield<SEXP> args, pass_as_name;
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
  * We simple call parser_Function x; x(args.getdata());
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
