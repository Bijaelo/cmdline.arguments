


#include "parser/argument/argument.h"

namespace cmdline_arguments::parser::argument{
  Argument::Argument(vector<string> _flags,
                     string _name,
                     string _count_option,
                     string _narg,
                     Function _pfunc,
                     List pfunc_args) :
  Narg(_narg, _name), pfunc(_pfunc), name(_name), flags(_flags.begin(), _flags.end())
  {

  }
  Argument::Argument(set<string> _flags,
                     string _name,
                     string _count_option,
                     string _narg,
                     Function _pfunc,
                     List pfunc_args) :
  Narg(_narg, _name), pfunc(_pfunc), name(_name), flags(_flags)
  {

  }
  Argument::Argument(string _flags,
                     string _name,
                     string _count_option,
                     string _narg,
                     Function _pfunc,
                     List pfunc_args) :
    Narg(_narg, _name), pfunc(_pfunc), name(_name), flags({_flags})
  {

  }
  // Hvad kan vi være sikre på? Det er at vi modtager arguments.
  // Når vi modtager et argument, så skal vi bruge vores option for hvordan dens længde skal vurderes
  // Så derfor må jeg havde implemneteret den option.

  // Skal det være et objekt, enum, string? Enum > string.

  // Hvor ofte skal det bruges?


  void Argument::add(const vector<string>& rawArgs){
    // can_take()
  }
  void Argument::add(const string& rawArg){
    // Gotta think how this should be handled.
    // I thought -f -f A should be "" "A", but that might cause problems.
    R_xlen_t n = (rawArg == "" ? 0 : 1);
    if(can_take(n)){

    }else
      stop("[arg: %s] received arguments but is incapable of taking input.", this -> name);
  }
  bool Argument::can_take(R_xlen_t n){

  }

  list<RObject> Argument::parse(){
    list<raw_argument>::iterator e = (this -> rawArgs).end();
    for(list<raw_argument>::iterator i = (this -> rawArgs).begin(); i != e; i++){
      parsedArgs.push_back(pfunc((*i).digest()));
    }
    return this -> parsedArgs;
  }


}
