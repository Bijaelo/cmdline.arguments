

#ifndef cmdline_arguments_argument_locator_h
#define cmdline_arguments_argument_locator_h
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
/* argument_locator class
 *
 * Takes in a vector of strings (commandline arguments) and
 * locates positional and flagged arguments without caring for their "validity".
 * Location is based on second argument "lookUps".
 */
namespace cmdline_arguments::parser{

  class argument_locator {
    const vector<string> rawArgs; // input vector
    list<vector<string>> lookups_list; // lookup values.
    map<string, list<vector<string>>> argLookup; // container for parsed arguments

    // Check a single argument to see if it contains a flag identifier
    inline bool isFlag(const string& arg) const {
      string subArg;
      size_t strSize = arg.size();
      // Iterate over all "sizes" of lookups (likely "-" and "--", but we allow more)
      for(auto lookups : lookups_list){
        size_t lookupSize = lookups.size();
        if(lookupSize <= strSize){
          subArg = arg.substr(0, lookupSize);
          // iterate over each individual lookup in lookups
          // likely just length 1, but we allow more.
          for(auto lookup : lookups){
            // check whether the substr is a flag
            string i = lookup;
            if(subArg == i){
              return true;
            }
          }
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
      auto b =  rawArgs.begin();
      if(start != b){
        vector<string> positionals(rawArgs.begin(), start);
        positionals.shrink_to_fit(); // we know it will never need more space.
        // Maybe there's a better way to do this.. But this work.. I'll trust the compiler.
        list<vector<string>> posList;
        posList.push_back(positionals);
        argLookup["positionals"] = posList;
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
        auto hasEqual = flagpos -> find('=');
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
        argLookup[curFlag].push_back(argVals);
      }
    }

  public:
    // args gets killed (at least until the implementation is fully done)
    // so cannot pass by reference
    argument_locator(const vector<string> args, vector<string>& _lookups):
       rawArgs(args){

        // generate lookup list sorted by size.
        sort(_lookups.begin(), _lookups.end());
        size_t curSize = 0;
        vector<string> curLookup;
        for(auto i : _lookups){
          if(i.size() != curSize){
            lookups_list.push_back(curLookup);
            curLookup.clear();
          }
          curLookup.push_back(i);
        }
        if(curLookup.size() > 0)
          lookups_list.push_back(curLookup);
        // Find flags

        vector<string>::const_iterator start = rawArgs.begin(),
                                       end = rawArgs.end();
        find_positionals(start, end);
        find_flags(start, end);
      };
    /* Pop out an element from argLookup, removing the element from argLookup
     *
     * @param key - A key to "find".
     * @returns a list with vectors of strings.
     *          For example the argument -f passed as "-f a -f a b" would return
     *          list<vector<string>> {{a}, {a, b}}, but calling pop again would
     *          return {{}}
     */
    inline list<vector<string>> pop(const string& key){
      auto e = argLookup.end(),
        i = argLookup.find(key);
      if(i == e)
        return list<vector<string>>(); // return empty list.
      list<vector<string>> o = (i -> second);
      argLookup.erase(i); // remove element
      return o;
    }

    /* Get an element from argLookup, removing the element from argLookup
     *
     * @param key - A key to "find".
     * @returns a list with vectors of strings.
     *          For example the argument -f passed as "-f a -f a b" would return
     *          list<vector<string>> {{a}, {a, b}}
     */
    inline list<vector<string>> get(const string& key){
      auto e = argLookup.end(),
        i = argLookup.find(key);
      if(i == e)
        return list<vector<string>>();
      return i -> second;
    }
    inline list<vector<string>> operator[](const string& key){
      return get(key);
    }
    /* Test if an element is contained in argLookup
     *
     * @param key a key to "find"
     * @returns bool if key is present, false otherwise
     */
    inline bool contains(const string& key){
#if __cpluplus > 201703L // contains doesn't exist before c++20, and potentially not in some compilers (guarded).
      return argLookup.contains(key);
#else
      return argLookup.count(key) != 0; // because: https://stackoverflow.com/a/15792245/10782538
#endif
    }

  };

}
#endif
