

// Typename. Code stolen from a post on stackoverflow.

//#ifndef cmdline_typename
//#define cmdline_typename

#include <type_traits>
#include <typeinfo>

#ifndef _MSC_VER
#   include <cxxabi.h>
#endif

#include <memory>
#include <string>
#include <cstdlib>

using std::string;


// den kan ikke finde string type_name<string>. But why can't it?

namespace cmdline_arguments::utils{
  template <typename T>
  string type_name();
  template<>
  string type_name<string>();

}

//#endif
