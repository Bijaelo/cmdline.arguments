
/* argument_container class for enclosing all operations on multiple arguments.
 * Comparable with the "parser" class in pythons argparse module.
 */

#ifndef cmd_args_argument_container
#define cmd_args_argument_container

#include <string>
#include <unordered_map>

namespace cmd_args::parser{
  class argument_locator;
  namespace argument{
    class argument; // remove later on for compiler optimizations
    class argument_container {

    public:
      argument_container() = default;
      argument_container(argument_container& rhs) = default;
      argument_container(argument_container&& rhs) = default;

    private:
      std::unordered_map<std::string, argument> args;
    };
  }
}

#endif
