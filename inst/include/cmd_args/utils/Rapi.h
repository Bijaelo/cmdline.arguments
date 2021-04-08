
#ifndef cmd_Rapi
#define cmd_Rapi
#include <Rinternals.h>
namespace cmd_args::utils::Rapi{
  namespace utils{
    namespace Rapi{
      //template <typename T>
      inline bool cmd_ISNULL(SEXP x){
        return x == R_NilValue || TYPEOF(x) == NILSXP;;
      }
    }
  }
}
#endif
