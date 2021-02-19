
#include "utils/Rapi.h"

namespace cmdline_arguments::utils::Rapi{
  //template <typename T>
  bool cmd_ISNULL(SEXP sexp){
    return sexp == R_NilValue || TYPEOF(sexp) == NILSXP;
  }
}
