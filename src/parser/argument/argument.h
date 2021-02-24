


// Argument class. Container for holding one or more rawArgs, narg and

#ifndef cmdline_arguments_argument_h
#define cmdline_arguments_argument_h

#include <Rcpp.h>
#include <list>
#include <vector>
#include <string>
#include <set> // for storing unique flags.
#include <iterator>
#include "parser/narg/narg.h"
#include "parser/argument/raw_argument.h"
#include "parser/pfunc/pfunc.h"
using namespace cmdline_arguments::parser;
using std::string, std::list, std::set, std::greater;
using Rcpp::Function, Rcpp::RObject, Rcpp::stop;

namespace cmdline_arguments::parser::argument{
  class Argument {
    const narg Narg;
    parserFunction pfunc;
    const string name;
    const set<string> flags; // <== maybe order will be locale affected. (Only matters if it screws positional arguments)
    list<raw_argument> rawArgs;
    // We don't know the return type of pfunc, so we need to shield it in RObject.
    list<RObject> parsedArgs;

    /* Options:
     * 1) Skal nye argumenter håndteres individuelt eller som en fælles vektor?
     * 2) Hvordan skal argumenterne leveres til pfunc? bliver viderledt til rawArgs.)
     * 3) hvordan argumenterne skal optælles (afledt af 1)
     *    Hvis de håndteres individuelt så er "count" = antal flags
     *    ellers er "count" = antal elementer
     *    Dette gives til Narg før storing.
     *
     * Er der andre?
     * 4*) Lazy evaluation? Det er vel egentlig først I selve Parser objektet.
     * Hvordan håndteres lazy evaluation?
     * Returner NULL ligesom i en normal liste, hvis argumentet ikke eksisterer?
     * Det tror jeg.
     *
     * Okay lazy evaluation, holdes i Parser objektet. Det subsettes med enten
     * "numeric", "integer" eller "string". Numeric afrundes og bruges som integer,
     * integer trækker argument i kronologisk rækkefølge jf. hvornår args blev lagt ind.
     * String trækker ud fra navnet.
     * Så det er ikke noget jeg behøver gøre lige nu.
     * Præcis hvordan den her subset skal implementeres, er ikke helt klart endnu.
     * Det tager jeg til den tid.
     *
     * Så jeg tror ikke der er nogen andre options der er krævede her.
     *
     * Dertil skal jeg være opmærksom på, at når pfunc kaldes,
     * skal det indkapsles i en try-catch, så en error kan bliver kastet
     * forsvarligt.
     *
     * Her har jeg ikke tænkt på flags endnu, men jeg tror at det simpleste for
     * håndtering af flags, er at gøre det afledt ud fra de resterende parametre
     * så et argument aldrig er et "flag" men at det i stedet har en række
     * options, der gør det til et flag. Tilsvarende for andre parametre.
     *
     * Indtil videre ignorere jeg også lige hvordan vi håndtere "action".
     * Først og fremmest vil jeg godt kunne håndtere standard argumenter,
     * og så kan jeg tage et kig på de andre typer af argumenter når det virker.
     *
     * Så action = "store", med forskellige narg og parser options.
     *
     * Flow:
     * gets created (raw args not given)
     *   -> initialize narg,
     *   -> initialise pfunc with args.
     * raw args added
     *   -> Use 3) to pass int to narg.add
     *   -> append to rawArgs
     * digest called
     *   -> iterate over raw_argument and parse string using pfunc.
     *
     */
  public:
    // constructors take copied values not addresses, as this might be the "first" time we store these values.

    // flags, Name, new_argument_count_option, pfunc, pfunc_args, raw_argument_option,
    Argument(vector<string>, string, string, string, Function, List);
    Argument(set<string>, string, string, string, Function, List);
    Argument(string, string, string, string, Function, List);

    void add(const vector<string>&);
    void add(const string&);
    void add();
    // Function called from add, to ensure that we can digest an extra n arguments.
    // What R_xlen_t n is depends on our new_argument_count_option.
    bool can_take(R_xlen_t);

    list<RObject> parse();

  };
}


#endif
