


// raw_argument container
/* raw_Container is the primary wrapper for handling addition of new arguments
 * such as when arguments are passed multiple times "-f a b -f c" or just
 * individually.
 *
 * In time it might become the actual argument_class but at the moment, I believe
 * these two needs to be separated. This container does not "parse" arguments,
 * but simply appends raw arguments on top of existing arguments.
 *
 *
 */



#ifndef cmdline_raw_container_h
#define cmdline_raw_container_h

#include <string>
#include <vector>
#include <list>
#include <Rcpp.h>
#include "utils/converters/converters.h"
#include "utils/traits/traits.h"
#include "parser/argument/raw_argument.h"
#include "parser/narg/narg.h"

using Rcpp::wrap, Rcpp::as, Rcpp::List,
  Rcpp::CharacterVector;

using std::string, std::vector, std::list;

using cmdline_arguments::traits::input_types,
    cmdline_arguments::traits::input_types::individual_input_type,
    cmdline_arguments::traits::input_types::vector_input_type,
    cmdline_arguments::traits::input_types::list_input_type,
    cmdline_arguments::traits::listOptions::individual,
    cmdline_arguments::traits::listOptions::vectors,
    cmdline_arguments::traits::listOptions::combine,
    cmdline_arguments::utils::convert_type,
    cmdline_arguments::utils::listOption,
    cmdline_arguments::parser::argument::raw_argument,
    cmdline_arguments::parser::narg;


namespace cmdline_arguments::parser::argument{
  // Needs a container
  class raw_Container {

    const input_types raw_option;
    list<raw_argument> rawList;
    const string name, outputName;
    listOption lOption;
    bool init = false;
    narg _narg;
  public:
    // inputType, name, outputName, listOption
    raw_Container(string _raw_option,
                  string _name,
                  string _outputName,
                  string _listOption,
                  string __narg)
        : raw_option(convert_type(_raw_option))
        , name(_name)
        , outputName(_outputName)
        , lOption(_listOption)
        , _narg(__narg, _name)
    { // Not narg crashing
    };
    // inputType, name, outputName, listOption
    raw_Container(input_types _raw_option,
                  string _name,
                  string _outputName,
                  string _listOption,
                  string __narg)
        : raw_option(_raw_option)
        , name(_name)
        , outputName(_outputName)
        , lOption(_listOption)
        , _narg(__narg, this -> name)
    {
    };
    // inputType, name, outputName, listOption
    raw_Container(input_types _raw_option,
                  string _name,
                  string _outputName,
                  string __narg)
        : raw_option(_raw_option)
        , name(_name)
        , outputName(_outputName)
        , _narg(__narg, this -> name)
    {
    };

    // add a new raw_argument
    inline void add(vector<string> _data){
      // need to include listOption handling. Either here by letting this be a seperate function.
      // I'll use a switch, but we could optimize further by storing the function pointer and only calling this.
      std::size_t n = _data.size();
      if(n == 0) // we don't have any arguments at all, this is a solo flag
        return;
      switch(lOption.getOption()){
        case individual:
          { // scope for x
            //_narg.add(n);
            for(auto i: _data){
              rawList.push_back(raw_argument(this -> raw_option,
                                             this -> name,
                                             {i},
                                             this -> outputName));
            }
            break;
          }
          break;
        case vectors:
          { // scope for x
            //_narg.add(1);
            rawList.push_back(raw_argument(this -> raw_option,
                                           this -> name,
                                           _data,
                                           this -> outputName));
            break;
          }
          break;
        case combine:
          if(init){ // init only for here (avoid calls to rawList.size unnecessarily).
            //_narg.add(1);
            rawList.push_back(raw_argument(this -> raw_option,
                                           this -> name,
                                           _data,
                                           this -> outputName));
            init = true;
          }else{
            rawList.back().add(_data);
          }
          break;
        default:
          Rcpp::stop("Undefined option specified for handling of flags provided multiple times [Given=?, valid={%s}]",
                     "individual, vector, combine");
      }
      return;
    };
    inline void add(string _data){
      vector<string> x({_data});
      add(x);
    };
    template<typename T>
    void operator+=(T _data){
      add(_data);
    }

    operator list<raw_argument>() const {
      return this -> rawList;
    }

    // Iterators -> will be changed later as this turns into the "argument" class.
    inline list<raw_argument>::const_iterator cbegin() const {return rawList.cbegin();}
    inline list<raw_argument>::const_iterator cend() const {return rawList.cend();}
    inline list<raw_argument>::const_iterator begin(){return rawList.begin();}
    inline list<raw_argument>::const_iterator end(){return rawList.end();}

  };

}


#endif
