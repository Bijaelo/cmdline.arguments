
#include "utils/Rapi.h"

namespace cmd_args::utils::Rapi{
  namespace utils{
    namespace Rapi{
      //template <typename T>
      bool cmd_ISNULL(SEXP sexp){
        return sexp == R_NilValue || TYPEOF(sexp) == NILSXP;
      }
    }
  }
}
