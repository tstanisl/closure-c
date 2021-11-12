#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

//#include <functional>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

#define CLOSURE_CALL_IMPL_(args, arg0, ...) ((*(arg0)) args)
#define CLOSURE_CALL(...) CLOSURE_CALL_IMPL_((__VA_ARGS__), __VA_ARGS__, ~)

//#define const
typedef const void *closure_t;

typedef double cb_t(closure_t, double);
typedef double fun_t(double);


/*
__attribute__((always_inline))
__attribute__((flatten))
static inline double square(double x) { return x * x; }
*/

double foo(double x) {
  struct deriv_closure {
    fun_t * const fun;
    double eps;
    cb_t * const closure;
  };

  double square(double x) { return x * x; }
  double deriv_closure_cb(closure_t closure, double x) {
    struct deriv_closure *cap = container_of(closure, struct deriv_closure, closure);
      fun_t *f = cap->fun;
      //fun_t *f = square;
      double eps = cap->eps;
    //return (f(x + eps) - f(x - eps)) / (2 * eps);
    return f(x);
  }

  #define DERIV_CLOSURE(f_, eps_) \
      (struct deriv_closure){ .fun = (f_), .eps = (eps_), .closure = deriv_closure_cb }

	const struct deriv_closure closure = DERIV_CLOSURE(square, 1e-3);
	return CLOSURE_CALL(&closure.closure, x);
}


/*
double bar(double x) {
    //std::function<double(double)>
    auto
    deriv = [f = square, eps=1e-3](double x) {
        return (f(x + eps) - f(x - eps)) / (2 * eps);
    };
    return deriv(x);
}
*/

double use_foo(void) { return foo(3.14); }
//double use_bar(void) { return bar(3.14); }

