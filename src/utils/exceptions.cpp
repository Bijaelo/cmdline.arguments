
#include "utils/exceptions.h"
template <typename... Args>
inline void NORET stopv(const char* fmt, int verbosity, int required_v, Args&&... args){
  if(verbosity >= required_v){
    Rcpp::stop(fmt, std::forward<Args>(args)...);
  }
  return;
}


template <typename... Args>
inline void NORET warningv(const char* fmt, int verbosity, int required_v, Args&&... args){
  if(verbosity >= required_v){
    Rcpp::warning(fmt, std::forward<Args>(args)...);
  }
  return;
}
