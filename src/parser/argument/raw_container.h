


// raw_argument container
// More or less a simple wrapper around a List, but takes care of insertion of new
// arguments, including whether arguments should be "appended" or create a entirely
// new raw argument. Further implements an iterator to go over each raw_argument.


#ifndef cmdline_raw_container_h
#define cmdline_raw_container_h

#include <string>
#include <vector>
#include <list>
#include <Rcpp.h>
#include "utils/converters/converters.h"
#include "utils/traits/traits.h"
#include "parser/argument/raw_argument.h"

using Rcpp::wrap, Rcpp::as, Rcpp::List,
Rcpp::CharacterVector, Rcpp::Shield;

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
    cmdline_arguments::parser::argument::raw_argument;


namespace cmdline_arguments::parser::argument{
  // Needs a container
  class raw_Container {

  public:
    // inputType, name, outputName, listOption
    raw_Container(string _raw_option, string _name, string _outputName, string _listOption):
        raw_option(convert_type(_raw_option)), name(_name), outputName(_outputName), lOption(_listOption){
    };
    // inputType, name, outputName, listOption
    raw_Container(input_types _raw_option, string _name, string _outputName, string _listOption)
      : raw_option(_raw_option), name(_name), outputName(_outputName), lOption(_listOption){
    };
    // inputType, name, outputName, listOption
    raw_Container(input_types _raw_option, string _name, string _outputName)
      : raw_option(_raw_option), name(_name), outputName(_outputName){
    };

    // add a new raw_argument
    inline void add(vector<string> _data){
      // need to include listOption handling. Either here by letting this be a seperate function.
      raw_argument x(this -> raw_option,
                     this -> name,
                     _data,
                     this -> outputName);
      rawList.push_back(x);
    };
    inline void add(string _data){
      add({_data});
    };
    template<typename T>
    void operator+=(T _data){
      add(_data);
    }
    operator list<raw_argument>(){
      return this -> rawList;
    }

    // Iterators
    inline list<raw_argument>::const_iterator cbegin(){return rawList.cbegin();}
    inline list<raw_argument>::const_iterator cend(){return rawList.cend();}

  private:
    const input_types raw_option;
    list<raw_argument> rawList;
    const string name, outputName;
    const listOption lOption;


  };

}


#endif
