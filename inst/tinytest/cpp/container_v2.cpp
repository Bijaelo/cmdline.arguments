/*
 * Test for rebuilding argument container using string_view containers.
 * Currently V1 did not succesfully
 */

// [[Rcpp::plugins("cpp2a")]]

// [[Rcpp::depends(cmdline.arguments)]]

#include <Rcpp.h>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <string_view>
#include <list>
#include <cmd_args/parser/argument/argument.h>
#include <cmd_args/parser/argument/locator.h>
#include <cmd_args/utils/options.h>

using namespace cmd_args::utils;
using namespace cmd_args::parser::argument;
using namespace std;
struct argument_container{
private:
  unordered_set<string_view> flags;
  unordered_multimap<string_view, cmd_argument&> mapper;
  list<cmd_argument> args;
  const bool allow_unknown_flags{false}, allow_duplicate_flags{true};


  cmd_args::parser::argument_locator locator;
  // Flag for handling if the same flag is provided muliple time?
  multi_flag_handling option{"Invalid option provided to parser for handling multiple flags"};

public:
  argument_container() {};
  argument_container(bool allow_unknown_flags, bool allow_duplicate_flags)
      : allow_unknown_flags(allow_unknown_flags),
        allow_duplicate_flags(allow_duplicate_flags){};


  template<typename T, typename O, typename F>
  inline void add_argument(const T& name,
                           const F& flags, // Note flags is here, so we need  to go this -> flag for all assignments
                           const T& meta,
                           const T& action,
                           const O& option,
                           const T& choices){
    /* Add argument takes the same parameters as the initializer of cmd_args::parser::argument::cmd_argument
     * It creates a new argument,
     * adds it to the back of the private member args,
     * goes through the flags contained in the created argument checking for duplicates (throwig an error if one is foud and allow_duplicate_flags is false)
     * and adds each flag to the private member set "flags" if no duplicate flags are found.
     * after that is done, a mapper from the argument to the result slot is placed in mapper.
     */
    args.emplace_back(name, flags, meta, action, option, choices);
    auto& a = args.back();
    auto& f = a.flags();
    auto e = f.end();
    if(!allow_duplicate_flags){
      for(auto b = f.begin(); b != e; b++){
        if((this -> flags).find(*b) == (this -> flags).end()){
          (this -> flags).emplace(*b);
        }else{
          for(auto b1 = f.begin(); b1 != b; b1++){
            (this -> flags).erase(*b1);
          }
          args.pop_back();
          Rcpp::stop("Duplicate flag");
        }
      }
    }else{
      for(auto b = f.begin(); b != e; b++)
        (this -> flags).emplace(*b);
    }
    mapper.insert({a.name(), a});
  }
  const unordered_set<string_view>& getflags() const {
    return flags;
  }
  const list<cmd_argument>& getargs() const {
    return args;
  }
  const unordered_multimap<string_view, cmd_argument&> getmapping() const {
    return mapper;
  }
};


// [[Rcpp::export]]
XPtr<argument_container> test_container_default_initializer(){
  return XPtr<argument_container>(new argument_container());
}


// [[Rcpp::export]]
XPtr<argument_container> test_container_initializer(bool allow_unknown_flags, bool allow_duplicate_flags){
  return XPtr<argument_container>(new argument_container(allow_unknown_flags, allow_duplicate_flags));
}


// [[Rcpp::export]]
void test_add_argument(XPtr<argument_container> Xptr,
                       const SEXP& name,
                       const SEXP& flags,
                       const SEXP& meta,
                       const SEXP& action,
                       const SEXP& option,
                       const SEXP& choices){
  Xptr -> add_argument(name,
                       flags,
                       meta,
                       action,
                       option,
                       choices);
}

// [[Rcpp::export]]
list<string> test_getflags(XPtr<argument_container> Xptr){
  list<string> out;
  auto flags = Xptr -> getflags();
  auto e = flags.end();
  for(auto b = flags.begin(); b != e; b++)
    out.push_back(string(*b));
  return out;
}

// [[Rcpp::export]]
size_t test_nargs(XPtr<argument_container> Xptr){
  return (Xptr -> getargs()).size();
}

// [[Rcpp::export]]
size_t test_nmappings(XPtr<argument_container> Xptr, string name){
  auto mapp = Xptr -> getmapping();
  return mapp.count(string_view(name));
}

