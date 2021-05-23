

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

namespace cmd_args{
  namespace utils{
    template <typename T>
    std::string type_name(){
      typedef typename std::remove_reference<T>::type TR;
      std::unique_ptr<char, void(*)(void*)> own
        (
#ifndef _MSC_VER
            abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                nullptr, nullptr),
#else
                                nullptr,
#endif
                                std::free
        );
      std::string r = own != nullptr ? own.get() : typeid(TR).name();
      if (std::is_const<TR>::value)
        r += " const";
      if (std::is_volatile<TR>::value)
        r += " volatile";
      if (std::is_lvalue_reference<T>::value)
        r += "&";
      else if (std::is_rvalue_reference<T>::value)
        r += "&&";
      return r;
    }
    template<>
    std::string type_name<std::string>()
    {
      typedef typename std::remove_reference<std::string>::type TR;
      std::unique_ptr<char, void(*)(void*)> own
        (
#ifndef _MSC_VER
            abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                nullptr, nullptr),
#else
                                nullptr,
#endif
                                std::free
        );
      std::string r = own != nullptr ? own.get() : typeid(TR).name();
      if (std::is_const<TR>::value)
        r += " const";
      if (std::is_volatile<TR>::value)
        r += " volatile";
      if (std::is_lvalue_reference<std::string>::value)
        r += "&";
      else if (std::is_rvalue_reference<std::string>::value)
        r += "&&";
      return r;
    }
  } //~utils
} // ~cmd_args

//#endif
