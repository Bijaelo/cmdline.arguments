
/* argument_container class for enclosing all operations on multiple arguments.
 * Comparable with the "parser" class in pythons argparse module.
 */

#ifndef cmd_args_argument_container
#define cmd_args_argument_container

#include <Rcpp.h>
#include <cmd_args/parser/argument/locator.h>
#include <cmd_args/parser/argument/argument.h>
#include <cmd_args/utils/options.h>


using namespace cmd_args::utils;
using namespace cmd_args::parser::argument;
using namespace std;
namespace cmd_args::parser::argument{
  class argument_container {

    /* multimap allows our argument to map duplicate names to the same slot in our outcome list.
     * We only need to store a pointer to the argument here, as they are stored individually in a list.
     */
    unordered_multimap<string, cmd_argument*> arg_list;
    // This list just stores the arguments without identifiers.
    list<cmd_argument> args; // <=== These are dying after initialization
    // Container for all known flags
    unordered_set<string> flags;
    // Should flags not contained in the parser result in an error?
    const bool allow_unknown_flags = false,
      // Should duplicate flags be allowed? (Note: There is no performance degration for this).
      allow_duplicate_flags = false;
      // ready to indicate all arguments has been parsed.
    bool ready = false;


    // locator is used to find arguments from a string (eg. from base::commandArgs() in R)
    argument_locator arg_loc;
    // Flag for handling if the same flag is provided muliple time?
    multi_flag_handling option{"Invalid option provided to parser for handling multiple flags"};


    inline void extract_argument_info(cmd_argument& arg){
      // There's an error here:
      // 1) The same flag is duplicated multiple times (why?)
      // 2) The flags disappear from our flag container as we add more flags to it.
      // It indicates 1) that I am not iterating appropriately
      // 2) That I accidently erase flags incorrectly. (This does not seem to be the case.. Not entirely sure what is happening here.)
      auto argflags = arg.flags();
      if(!allow_duplicate_flags){
        auto e = flags.end();
        for(auto i : argflags){
          if(flags.find(i) != e){
            // If we find a duplicate key we'll have to rollback any changes.
            string dub(i), dubname(arg.name());
            for(auto ii = argflags.begin(); (*ii) != i; ii++){
              string fldub(*ii);
              flags.erase(fldub);
            }
            args.pop_back();
            Rcpp::stop("[Arg: %s] Duplicate flag found in provided argument (%s)", dubname, dub);
            return;
          }
          flags.emplace(i);
        }
      }else{
        for(auto i = argflags.begin(); i != argflags.end(); i++){
          Rcout << *i << " " << (flags.find(*i) != flags.end() ? "found" : "not found") << '\n';
          if(flags.find(*i) == flags.end())
            flags.emplace(string_view(*i));
        }
      }
      // add arguent to list
      arg_list.insert({string(arg.name()), &arg});
    }
  public:
    argument_container(){};
    argument_container(bool allow_unknown_flags, bool allow_duplicate_flags):
        allow_unknown_flags(allow_unknown_flags),
        allow_duplicate_flags(allow_duplicate_flags){}

    /* force parse all contained arguments
     *
     * Iteratively go through arguments and parse the
     */
    inline void parse(){
      for(auto arg : args)
        arg.parse();
    }


    // Add a single argument. All error handling is performed by Argument::add
    template<typename T, typename O, typename F>
    inline void add_argument(const T& name,
                             const F& flags, // Note flags is here, so we need  to go this -> flag for all assignments
                             const T& meta,
                             const T& action,
                             const O& option,
                             const T& choices){
      // Is it emplace back?
      args.emplace_back(name, flags, meta, action, option, choices);
      extract_argument_info(args.back());
    }

#if CMD_GCC_VERSION >= 11 || CMD_CLANG_VERSION  >= 12 // For custom unordered_map lookup (implemented in Clang and GCC trunk (12 and 11 respectively.))
    inline Rcpp::RObject get_argument(string_view key){
#else
    inline Rcpp::RObject get_argument(const string& key){
#endif
      // FIXME: need to actually do something.
      return Rcpp::wrap(R_NilValue);
    }

    inline const unordered_set<string>& getflags(){
      return flags;
    }

    inline const list<string_view> getflags2(){
      list<string_view> res;
      for(auto i : args){
        for(auto j : i.flags())
          res.push_back(j);
      }
      return res;
    }

    inline const int narguments(){
      return args.size();
    }

    inline const int nflags(){
      return flags.size();
    }
  };
}

#endif
