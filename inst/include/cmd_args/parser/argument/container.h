
/* argument_container class for enclosing all operations on multiple arguments.
 * Comparable with the "parser" class in pythons argparse module.
 */

#ifndef cmd_args_argument_container
#define cmd_args_argument_container

#include <string>
#include <unordered_map>
#include <cmd_args/parser/argument/locator.h>

namespace cmd_args::parser{
  namespace argument{
    struct argument_container {
      argument_locator parser;
      // Problem: I don't know the markers beforehand, so argument_locator needs a default constructor
      /* Currently trying to make a patch for this in locator.h
       * But for some reason removing const from rawArgs causes all sorts of problems with no matching call.
       * So I will have to look into this a bit.
       */
      argument_container(vector<string> rawArgs, vector<string> flags):parser(rawArgs){}

    private:

    };
  }
}

#endif
