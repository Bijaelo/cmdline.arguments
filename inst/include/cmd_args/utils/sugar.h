
#ifndef cmd_args_sugar_h
#define cmd_args_sugar_h

#include <Rcpp.h>
#include <list>
using namespace Rcpp;

namespace cmd_args{
  namespace utils{
    namespace sugar{

      template<typename T>
      inline bool anyDuplicated(const T& x){
        return is_true(any(duplicated(x)));
      }

      template<>
      inline bool anyDuplicated(const SEXP& x){
        switch(TYPEOF(x)){
          case INTSXP:
            return is_true(any(duplicated(Vector<INTSXP>(x))));
          case REALSXP:
            return is_true(any(duplicated(Vector<REALSXP>(x))));
          case STRSXP:
            return is_true(any(duplicated(Vector<STRSXP>(x))));
          default:
            stop("Invalid type in cmd_args::anyDuplicated [Type = %s, valid = (%s, %s, %s)]",
                 Rf_type2char(TYPEOF(x)),
                 Rf_type2char(INTSXP),
                 Rf_type2char(REALSXP),
                 Rf_type2char(STRSXP));
        }
      }

      template<typename T>
      inline bool allin(const T& x, const T& table){
        return is_true(all(in(x, table)));
      }

      template<>
      inline bool allin(const SEXP& x, const SEXP& table){
        if(TYPEOF(x) != TYPEOF(table))
          stop("x and table must be of the same type.");
        switch(TYPEOF(x)){
        case INTSXP:
          return is_true(all(Rcpp::in(Vector<INTSXP>(x), Vector<INTSXP>(table))));
        case REALSXP:
          return is_true(all(Rcpp::in(Vector<REALSXP>(x), Vector<REALSXP>(table))));
        case STRSXP:
          return is_true(all(Rcpp::in(Vector<STRSXP>(x), Vector<STRSXP>(table))));
        default:
          stop("Invalid type in cmd_args::allin [Type = %s, valid = (%s, %s, %s)]",
               Rf_type2char(TYPEOF(x)),
               Rf_type2char(INTSXP),
               Rf_type2char(REALSXP),
               Rf_type2char(STRSXP));
        }
      }

      template<typename T>
      inline bool same_lengths(const T& x){
        return same_lengths(wrap(x));
      }

      template<typename T>
      inline bool same_lengths(const std::list<T>& x){
        if(x.size() <= 1)
          return true;
        auto b = x.begin(), e = x.end();
        auto n = b -> size();
        b++;
        for(;b != e; b++){
          if((b -> size()) != n)
            return false;
        }
        return true;
      }

      template<>
      inline bool same_lengths(const SEXP& x){
        auto type = TYPEOF(x);
        if(type == VECSXP){
          R_xlen_t size = Rf_xlength(x);
          if(size <= 1)
            return true;
          R_xlen_t n = Rf_xlength(VECTOR_ELT(x, 0));
          for(R_xlen_t i = 1; i < size; i++){
            if(Rf_xlength(VECTOR_ELT(x, i)) != n)
              return false;
          }
        }else if(type == LISTSXP)
          stop("Lengths not defined for pairlists");
        return true;
      }

      template<typename T>
      inline bool is_StringOrChar(const T& x){
        return is_StringOrChar(wrap(x));
      }

      template<>
      inline bool is_StringOrChar(const SEXP& x){
        int n = Rf_xlength(x);
        if(n > 1)
          return false;
        int type = TYPEOF(x);
        if(type != STRSXP && type != CHARSXP)
          return false;
        return true;
      }

      inline bool is_StringOrCharAndNotNA(const SEXP& x){
        int type = TYPEOF(x);
        if(type == STRSXP){
          if(Rf_xlength(x) > 1)
            return false;
          if(STRING_PTR(x)[0] == NA_STRING)
            return false;
          return true;
        }else if(type == CHARSXP){
          if(x == NA_STRING)
            return false;
          return true;
        }
        return false;
      }
    }
  }
}
#endif
