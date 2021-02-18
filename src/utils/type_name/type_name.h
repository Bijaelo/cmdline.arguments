

// Typename. Code stolen from a post on stackoverflow.

#ifndef cmdline_typename
#define cmdline_typename

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
using std::string;


namespace cmdline_arguments::utils{
  template <class T>
  string type_name();
}

#endif
