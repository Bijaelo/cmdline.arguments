


#include "parser/argument/class.h"

namespace cmd_args{
  namespace parser{
    namespace argument{

      template<typename T>
      Argument::Argument(vector<string> _flags,
                         string _name,
                         string _action,
                         string _narg,
                         SEXP _help,
                         T _parserFunction,
                         List _parserArgs
                         ):
        Narg(_narg),
        pfunc(_parserFunction),
        name(_name),
        action(_action),
        nargRaw(_narg),
        flags(_flags.begin(), _flags.end()),
        pArgs(_parserArgs),
        help(_help)
        {
         testHelp();
        }
      template<typename T>
      Argument::Argument(string _flags,
                         string _name,
                         string _action,
                         string _narg,
                         SEXP _help,
                         T _parserFunction,
                         List _parserArgs
      ):
       Narg(_narg),
        pfunc(_parserFunction),
        name(_name),
        action(_action),
        nargRaw(_narg),
        flags({_flags}),
        pArgs(_parserArgs),
        help(_help)
      {
       testHelp();
      }
      void Argument::testHelp(){
        SEXP h = wrap(this -> help); // no need to protect as it is bound in an RObject
        if(!Rf_isString(h)){
          switch(TYPEOF(h)){
          case STRSXP:
          case CHARSXP:
            if(Rf_length(h) > 1)
              stop("[arg: %s] \"help\" must be either a string or a callable function. See the description section of \"help(add_argument)\" for more information.", this -> name);
          case CLOSXP:
          case SPECIALSXP:
          case BUILTINSXP:
            break;
          default:
            stop("[arg: %s] \"help\" must be either a string or a callable function. See the description section of \"help(add_argument)\" for more information.", this -> name);
          }
        }
      }

    } // ~argument
  } // ~parser
} // ~cmd_args
