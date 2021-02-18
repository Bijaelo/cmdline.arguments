

#ifndef cmdline_narg
#define cmdline_narg
#include <Rcpp.h>
#include <string.h>
#include <functional>
using std::string,
  std::function,
  std::stoi;
using Rcpp::stop;
/* Class for containing narg, parsing its value and comparing.
 *
 *
 * Requires C++11 o compile std::function. A function pointer could be used to
 * avoid this.
 *
 *
 *
 */

namespace cmdline_arguments::parser{

  class narg{
  private:
    // storage for raw "narg" and name (latter used for error message)
    // TODO: change name to a string reference, once name is stored in
    // an overall container safely (breaks when not stored safely)
    const string narg_raw, name;

    // store value.
    R_xlen_t current_value = 0, critical_value;

    // Comparison functions. Store the one we'll actually use in the function
    // pointer, to minimize
    typedef bool (narg::*cmpfun)(R_xlen_t& more) const;
    cmpfun compare_fun;
    //function<bool(R_xlen_t&)> compare_fun;
    bool le(R_xlen_t&) const,
      leq(R_xlen_t&) const,
      ge(R_xlen_t&) const,
      geq(R_xlen_t&) const,
      eq(R_xlen_t&) const;
    // Error message (set in parse_narg):
    string errormsg;
    void throwerr() const;

    // function for parsing input arguments.
    void parse_narg();

  public:
    // Initializer
    narg(const string, const string&);
    // comparison function
    // Test if we can accept 'R_xlen_t' more arguments.
    bool digest(R_xlen_t&) const;
    // Close narg,
    // Test that the bool is fulfilled, otherwise throw error.
    void close() const;

    void add(R_xlen_t&);

    R_xlen_t leftover();


    // Operator overload to "digest"
    bool operator()(R_xlen_t&) const;
    bool operator()() const;
    // Another way of calling "add".
    void operator+=(R_xlen_t&);
    void operator++();

    // getters
    string getnarg(),
      geterr(),
      getname();;
    R_xlen_t getv(), getc();
  };

}
#endif
