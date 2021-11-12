#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

#define CLOSURE_CALL_IMPL_(args, arg0, ...) ((*(arg0)) args)
#define CLOSURE_CALL(...) CLOSURE_CALL_IMPL_((__VA_ARGS__), __VA_ARGS__, ~)
typedef void *closure_t;

typedef double fun_t(closure_t, double);

struct derivative {
	fun_t **fun;
	double eps;
	fun_t *lambda;
};

static double derivative_cb(closure_t cls, double x) {
	struct derivative cap = *container_of(cls, struct derivative, lambda);
	double fa = CLOSURE_CALL(cap.fun, x - cap.eps);
	double fb = CLOSURE_CALL(cap.fun, x + cap.eps);
	double res = (fb - fa) / (2 * cap.eps);
	//printf("res[%lf]=%lf\n", x, res);
	return res;
}

#define derivative_init(fun_, eps_) ((struct derivative) { .fun = fun_, .eps = eps_, .lambda = derivative_cb })

static double cube_cb(closure_t _unused, double x) {
	(void)_unused;
	return sin(x);
}
int main() {
	const int n = 8;
	struct derivative D[n];

	fun_t *cube = cube_cb;

	fun_t **prev = &cube;
	for (int i = 0; i < n; ++i) {
		D[i] = derivative_init(prev, 1e-3);
		prev = &D[i].lambda;
	}

	double x = 0;
	printf("    f(%lf) = %lf\n", x, CLOSURE_CALL(&cube, x));
	for (int i = 0; i < n; ++i)
		printf("d^%d f(%lf) = %lf\n", i + 1, x, CLOSURE_CALL(&D[i].lambda, x));

	return 0;
}
