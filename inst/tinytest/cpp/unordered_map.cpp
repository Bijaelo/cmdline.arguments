

// [[Rcpp::plugins("cpp2a")]]



#include <Rcpp.h>
#include <unordered_set>
#include <string>
#include <string_view>
#include <list>
using namespace std;
using namespace Rcpp;

struct B{
  string a;
  B(SEXP x){
    a = CHAR(STRING_ELT(x, 0));
  }
};

struct A{
  unordered_set<string_view> set;
  list<B> bs;
  void add_string(SEXP x){
    bs.emplace_back(x);
    auto& b = bs.back();
    set.emplace(b.a);
  }
};

// [[Rcpp::export]]
XPtr<A> make_A(){
  XPtr<A> b(new A());
  return b;
}

// [[Rcpp::export]]
void add_string(SEXP x, XPtr<A> ptr){
  if(TYPEOF(x) != STRSXP || Rf_xlength(x) != 1)
    stop("Only strings allowed");
  ptr -> add_string(x);
}

// [[Rcpp::export]]
SEXP print_set(XPtr<A> ptr){
  vector<string> res;
  for(auto i: (ptr -> set)){
    Rcout << i << " ";
    res.push_back(string(i));
  }
  Rcout << '\n';
  return wrap(res);
}

/***R

A <- make_A()
A2 <- make_A()
s1 <- "-t"
s2 <- "--test"
s3 <- "--foo"
add_string(s1, A)
add_string(s1, A2)
print_set(A)
print_set(A2)
add_string(s2, A)
add_string(s2, A2)
print_set(A)
print_set(A2)
add_string(s3, A)
add_string(s3, A2)
print_set(A)
print_set(A2)
*/

