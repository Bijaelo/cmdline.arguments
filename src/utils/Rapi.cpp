
#include "utils/Rapi.h"

namespace cmdline_arguments::utils::Rapi{
  //template <typename T>
  bool cmd_ISNULL(SEXP sexp){
    return TYPEOF(sexp) == NILSXP;
  }
}
