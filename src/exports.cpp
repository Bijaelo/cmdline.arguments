

//#include <arguments.h> // currently non-existent
//#include <parser.h> // currently non-existent

#include <R.h>

#include <R_ext/Rdynload.h>
// borrowed from Matrix
#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}
#define EXTDEF(name)  {#name, (DL_FUNC) &name, -1}

// Create a list of pointers to be exported to R
static const R_CallMethodDef CallEntries[] = {

  {NULL, NULL, 0}
};
// Register the pointers as Dynamic Symbols for R.
extern "C" void R_init_cmdline_arguments(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, (Rboolean)FALSE);
}
