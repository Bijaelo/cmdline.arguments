

#ifndef cmd_args_argument_locator_h
#define cmd_args_argument_locator_h
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <functional>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cmd_args/utils/macros.h>
using namespace std;


/* argument_locator class
 *
 * Takes in a vector of strings (commandline arguments) and
 * locates positional and flagged arguments without caring for their "validity".
 * Location is based on second argument "lookUps".
 *
 *
 * It needs an iterator to parsedArgs
 */
namespace cmd_args::parser{
#if CMD_GCC_VERSION >= 11 || CMD_CLANG_VERSION  >= 12
  struct string_hash
  {
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    size_t operator()(const char* str) const        { return hash_type{}(str); }
    size_t operator()(std::string_view str) const   { return hash_type{}(str); }
    size_t operator()(std::string const& str) const { return hash_type{}(str); }
  };
#endif
  class argument_locator {
    vector<string> rawArgs; // input vector
    //list<vector<string>> flag_lookups; // lookup values.
#if CMD_GCC_VERSION >= 11 || CMD_CLANG_VERSION  >= 12
    unordered_set<std::string, string_hash, std::equal_to<>> flag_lookups; // <== change from flag_lookups
#else
    unordered_set<std::string> flag_lookups;
#endif
    set<size_t, std::greater<size_t>> sizes;
    unordered_set<std::string>::iterator e;
    unordered_map<string, list<vector<string>>> parsedArgs;
    bool argsAdded = false, argsParsed = false;
    size_t minSize;
    // Check a single argument to see if it contains a flag identifier
    inline bool isFlag(const string& arg) const {
      size_t strSize = arg.size();
      for(auto size : sizes){
        if(size < strSize){
          /* Once gcc-11 and/or clang 12 is released (current trunk) string_view(const CharT *, size_t) and heterogeneous lookups should exist,
           * so we can reduce computation time slightly by avoiding a string copy.
           *
           * Using godbolt shows
           * string_view sstr(arg.c_str(), size);
           * and
           * string_view sstr(arg.begin(), arg.begin() + size);
           * is equivalent in both clang and gcc
           */
#if CMD_GCC_VERSION >= 11 || CMD_CLANG_VERSION >= 12
          string_view sstr(arg.c_str(), size);
          if(flag_lookups.find(sstr) != e)
#else
          if(flag_lookups.find(arg.substr(0, size)) != e)
#endif
            return true;
        }
      }
      return false;
    };

    inline void find_next_flag(vector<string>::const_iterator& from,
                               vector<string>::const_iterator& end) const {
      while(from != end){
        if(isFlag(*from)) break; // maybe the dereference will break ? got to check.
        from++;
      }
    };

    inline void find_positionals(vector<string>::const_iterator& start,
                                 vector<string>::const_iterator& end) {
      find_next_flag(start, end);
      auto b =  rawArgs.cbegin();
      if(start != b){
        // Maybe we could cut this down... I am not sure how yet.
        vector<string> positionals(b, start);
        positionals.shrink_to_fit(); // we know it will never need more space.
        // Maybe there's a better way to do this.. But this work.. I'll trust the compiler.
        list<vector<string>> posList;
        posList.push_back(positionals);
        parsedArgs["positionals"] = posList;
      }
    };

    // Iterate through rawArgs and find args with "flags".
    inline void find_flags(vector<string>::const_iterator& start,
                           vector<string>::const_iterator& end){
      // start will **always** either be end or a flag
      // because we always start by inding positionals!
      while(start != end){
        vector<string>::const_iterator flagpos = start;
        vector<string> argVals;
        start++;
        find_next_flag(start, end);
        // check if flag position contains a value
        auto hasEqual = flagpos -> find('=', minSize);

        string curFlag;
        // Wtf did i think when i wrote this?
        if(hasEqual != string::npos){
          // We found an equal sign. Extract the value following the equal sign.
          string equalVal = flagpos -> substr(hasEqual + 1);
          if(equalVal.size() != 0)
            argVals.push_back(equalVal);
          curFlag = flagpos -> substr(0, hasEqual);
        }else
          curFlag = *flagpos;
        // append values only if we have any "values".
        // Performs 1 redundant check for "end" but who cares.
        if(start != flagpos + 1){
          argVals.insert(argVals.end(), flagpos + 1, start);
        }
        argVals.shrink_to_fit();
        // will this work? This will definitely have to be tested,
        // before we know.
        parsedArgs[curFlag].push_back(argVals);
      }
    }

  public:
    // args gets killed (at least until the implementation is fully done)
    // so cannot pass by reference
    argument_locator(const vector<string> args, vector<string>& _lookups):
       rawArgs(args){
      argsAdded = true;
      argsParsed = false;
      parse(_lookups);
        // generate lookup list sorted by size.
    };
    argument_locator(const vector<string> args):rawArgs(args){
      argsAdded = true;
    };
    argument_locator(){};

    inline void insert(const vector<string>(args)){
      rawArgs.insert(rawArgs.end(), args.begin(), args.end());
      argsAdded = true;
      argsParsed = false;

    }
    // reset all containers and flags
    inline void clear(){
      rawArgs.clear();
      flag_lookups.clear();
      sizes.clear();
      argsAdded = false;
      argsParsed = false;
    }

    /* parse eisting arguments looking for "lookup" as flag indicator
     * Doesn't evaluate on the second execution, unless additiona args are passed first, or
     */
    inline void parse(vector<string>& _lookups){
      if(argsAdded && !argsParsed){
        if(_lookups.size() == 0)
          throw invalid_argument("empty lookups provided for locator");
        for(const auto& i: _lookups){
          flag_lookups.insert(i);
          sizes.insert(i.size());
        }
        parsedArgs.clear();
        // Extract the "smallest" size of flag_lookups (the last value in sizes).
        minSize = *(sizes.rbegin());
        vector<string>::const_iterator start = rawArgs.begin(),
          end = rawArgs.end();
        find_positionals(start, end);
        find_flags(start, end);
        argsParsed = true;
      }
    }
    inline bool is_parsed(){
      return argsParsed;
    }
    inline operator bool(){
      return argsParsed;
    }
    inline bool has_flags(){
      return argsAdded;
    }
    inline unordered_map<string, list<vector<string>>> get_parsedArgs(){
      if(argsParsed){
        return parsedArgs;
      }else{
        throw "locator::get_parsedArgs called prior to locator::parse";
      }
    }
    /* Pop out an element from parsedArgs, removing the element from parsedArgs
     *
     * @param key - A key to "find".
     * @returns a list with vectors of strings.
     *          For example the argument -f passed as "-f a -f a b" would return
     *          list<vector<string>> {{a}, {a, b}}, but calling pop again would
     *          return {{}}
     */
    inline list<vector<string>> pop(const string& key){
      if(!argsParsed)
        return list<vector<string>>();
      auto e = parsedArgs.end(),
        i = parsedArgs.find(key);
      if(i == e)
        return list<vector<string>>(); // return empty list.
      list<vector<string>> o = (i -> second);
      parsedArgs.erase(i); // remove element
      return o;
    }

    /* Get an element from parsedArgs, removing the element from parsedArgs
     *
     * @param key - A key to "find".
     * @returns a list with vectors of strings.
     *          For example the argument -f passed as "-f a -f a b" would return
     *          list<vector<string>> {{a}, {a, b}}
     */
    inline list<vector<string>> get(const string& key){
      if(!argsParsed)
        return list<vector<string>>();
      auto e = parsedArgs.end(),
        i = parsedArgs.find(key);
      if(i == e)
        return list<vector<string>>();
      return i -> second;
    }
    inline list<vector<string>> operator[](const string& key){
      return get(key);
    }
    /* Test if an element is contained in parsedArgs
     *
     * @param key a key to "find"
     * @returns bool if key is present, false otherwise
     */
    inline bool contains(const string& key){
      if(!argsParsed)
        return false;
#if __cpluplus > 201703L // contains doesn't exist before c++20, and potentially not in some compilers (guarded).
      return parsedArgs.contains(key);
#else
      return parsedArgs.count(key) != 0; // Based on suggestions from: https://stackoverflow.com/a/15792245/10782538
#endif
    }
    // Allow for simple iteration through arguments.
    unordered_map<string, list<vector<string>>>::iterator begin(){
      return parsedArgs.begin();
    }
    unordered_map<string, list<vector<string>>>::const_iterator cbegin(){
      return parsedArgs.cbegin();
    }
    unordered_map<string, list<vector<string>>>::iterator end(){
      return parsedArgs.end();
    }
    unordered_map<string, list<vector<string>>>::const_iterator cend(){
      return parsedArgs.cend();
    }

  };
} // ~cmd_args
#endif
