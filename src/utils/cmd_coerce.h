

#ifndef cmdline_coerce_h
#define cmdline_coerce_h
// Rinternal break code compilation, because of the
// #include <Rinternals.h>
#include <R.h>
#include <Rcpp.h>
// #include <string>
using namespace Rcpp;

#define NAMED_BITS 16


// Extracted from Defn.h, Rinternals.h, coerce.h and so on.
// Some flags could make this unnecessary, but there are some conflicts with Rcpp
typedef SEXP (*CCODE)(SEXP, SEXP, SEXP, SEXP);

/* Information for Deparsing Expressions */
typedef enum {
  PP_INVALID  =  0,
  PP_ASSIGN   =  1,
  PP_ASSIGN2  =  2,
  PP_BINARY   =  3,
  PP_BINARY2  =  4,
  PP_BREAK    =  5,
  PP_CURLY    =  6,
  PP_FOR      =  7,
  PP_FUNCALL  =  8,
  PP_FUNCTION =  9,
  PP_IF       = 10,
  PP_NEXT     = 11,
  PP_PAREN    = 12,
  PP_RETURN   = 13,
  PP_SUBASS   = 14,
  PP_SUBSET   = 15,
  PP_WHILE    = 16,
  PP_UNARY    = 17,
  PP_DOLLAR   = 18,
  PP_FOREIGN  = 19,
  PP_REPEAT   = 20
} PPkind;

typedef enum {
  PREC_FN      = 0,
  PREC_LEFT    = 1,
  PREC_EQ      = 2,
  PREC_RIGHT   = 3,
  PREC_TILDE   = 4,
  PREC_OR      = 5,
  PREC_AND     = 6,
  PREC_NOT     = 7,
  PREC_COMPARE = 8,
  PREC_SUM     = 9,
  PREC_PROD    = 10,
  PREC_PERCENT = 11,
  PREC_COLON   = 12,
  PREC_SIGN    = 13,
  PREC_POWER   = 14,
  PREC_DOLLAR  = 15,
  PREC_NS      = 16,
  PREC_SUBSET  = 17
} PPprec;


typedef struct {
  PPkind kind;     /* deparse kind */
PPprec precedence; /* operator precedence */
unsigned int rightassoc;  /* right associative? */
} PPinfo;


typedef struct {
  char   *name;    /* print name */
CCODE  cfun;     /* c-code address */
int    code;     /* offset within c-code */
int    eval;     /* evaluate args? */
int    arity;    /* function arity */
PPinfo gram;     /* pretty-print info */
} FUNTAB;



extern FUNTAB   R_FunTab[];     /* Built in functions */

struct sxpinfo_struct {
  SEXPTYPE type      :  TYPE_BITS;
  /* ==> (FUNSXP == 99) %% 2^5 == 3 == CLOSXP
   * -> warning: `type' is narrower than values
   *              of its type
   * when SEXPTYPE was an enum */
  unsigned int scalar:  1;
  unsigned int obj   :  1;
  unsigned int alt   :  1;
  unsigned int gp    : 16;
  unsigned int mark  :  1;
  unsigned int debug :  1;
  unsigned int trace :  1;  /* functions and memory tracing */
  unsigned int spare :  1;  /* used on closures and when REFCNT is defined */
  unsigned int gcgen :  1;  /* old generation number */
  unsigned int gccls :  3;  /* node class */
  unsigned int named : NAMED_BITS;
  unsigned int extra : 32 - NAMED_BITS; /* used for immediate bindings */
}; /*		    Tot: 64 */

#define SEXPREC_HEADER           \
  struct sxpinfo_struct sxpinfo; \
  struct SEXPREC *attrib;        \
  struct SEXPREC *gengc_next_node, *gengc_prev_node

struct primsxp_struct {
  int offset;
};

  struct symsxp_struct {
    struct SEXPREC *pname;
    struct SEXPREC *value;
    struct SEXPREC *internal;
  };

  struct listsxp_struct {
    struct SEXPREC *carval;
    struct SEXPREC *cdrval;
    struct SEXPREC *tagval;
  };

  struct envsxp_struct {
    struct SEXPREC *frame;
    struct SEXPREC *enclos;
    struct SEXPREC *hashtab;
  };

  struct closxp_struct {
    struct SEXPREC *formals;
    struct SEXPREC *body;
    struct SEXPREC *env;
  };

  struct promsxp_struct {
    struct SEXPREC *value;
    struct SEXPREC *expr;
    struct SEXPREC *env;
  };


  typedef struct SEXPREC {
    SEXPREC_HEADER;
    union {
      struct primsxp_struct primsxp;
      struct symsxp_struct symsxp;
      struct listsxp_struct listsxp;
      struct envsxp_struct envsxp;
      struct closxp_struct closxp;
      struct promsxp_struct promsxp;
    } u;
  } SEXPREC;

#if defined(USE_RINTERNALS) && defined(CMDLINE_ARGUMENTS_MAYBE_IN_THE_FUTURE)
#define PRIMNAME(x)	(R_FunTab[(x)->u.primsxp.offset].name)
#endif


// because I still dont know how to get access to FunTab
namespace cmdline_arguments::utils::coerce {
  inline SEXP cmd_do_docall(SEXP, SEXP, Rcpp::Environment, std::string);
}
/*
cmdline_arguments::utils::coerce::cmd_do_docall(// SEXP
                                                SEXPREC*,
                                                // SEXP
                                                SEXPREC*,
                                                // Environment <- maybe this one.
                                                Rcpp::Environment_Impl<Rcpp::PreserveStorage>,
                                                // basic string
                                                std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)
*/
#endif
