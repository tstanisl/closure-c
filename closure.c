#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

typedef void *closure_t;

#define CLOSURE_DECLARE()
#define CLOSURE_TO_CONTEXT()
#define CLOSURE_FROM_CONTEXT()
#define CLOSURE_FIRST_(A, ...) A
#define CLOSURE_CALL(...) ((*(CLOSURE_FIRST_(__VA_ARGS__,~)))(__VA_ARGS__))
#define CLOSURE_TYPE(ID, RETTYPE, ARGS)

/*
 * CLOSURE_TYPE(int, fun);
 * int (*fun) (void *);
 *
 * CLOSURE_TYPE(float, fun, int);
 * float (*fun)(void*, int)
 */

//typedef DECLARE_CLOSURE(derivative_closure_t, double, (double));

#if 0


struct add_capture { int a; };

CLOSURE(add_closure_t, int, (int a), struct add_capture) {
	CLOSURE_CAPTURE(add_closure_t, ctx);
	return a + ctx->add;
}

int fun1() {
	CLOSURE_TYPE(closure, int, (int)) = CLOSURE_AUTO(add_closure_t, ({ .add = 2 }));
	return CLOSURE_CALL(closure, 3);
}

int fun2() {
	CLOSURE_TYPE(closure, int, (int)) = CLOSURE_HEAP(add_closure_t, ({ .add = 2 }));
	int ret = CLOSURE_CALL(closure, 3);
	CLOSURE_FREE(closure);
	return ret;
}
#endif

#if 0
struct adder_closure {
	int num;
	CLOSURE_ANCHOR(closure, int, (int));
};

CLOSURE_CALLBACK(adder_callback, int, (int a)) {
	struct adder_closure *cap = CLOSURE_CAPTURE(struct adder_closure, capture);
	return cap->num + a;
}

int fun1(CLOSURE_TYPE(closure, int, (int))) {
	return CLOSURE_CALL(closure, 2);
}

int fun() {
	struct adder_closure adder = { .closure = CLOSURE_INIT(adder_callback /*, dtor */), .num = 3 };
	return fun1(adder.closure);
}
#endif

#if 1
/*
CLOSURE(add_closure_t, int, (int a), int) {
	int num = CLOSURE_CAPTURE(add_closure_t);
	return a + num;
}

int fun1() {
	CLOSURE_TYPE(closure, int, (int)) = CLOSURE_AUTO(add_closure_t, 2);
	return CLOSURE_CALL(closure, 3);
}
*/


/*
struct adder_closure {
	int capture;
	int (*closure[2])(void *, int);
};

int adder_closure__callback(void* closure, int a) {
	int num = container_of(closure, struct adder_closure, closure)->capture;
	return num + a;
}

int fun1(int (**closure)(void*, int)) {
	return (*closure)(closure, 2);
}

//int (**fun(void))(void*, int) {
int fun(void) {
	struct adder_closure capture = { .capture = 3, .closure = { adder_closure__callback } };
	return fun1(capture.closure);
}
*/

/*
typedef struct closure_t closure_t;
typedef void closure_dtor(closure_t *closure);
struct closure_t {
	closure_dtor* dtor[2];
};
*/

typedef void (closure_dtor)(void *);
typedef int fii(void*, int);

struct adder_closure {
	int num;
	fii *closure[2];
};
/*
struct adder_closure {
	int num;
	CLOSURE_ARCHOR(closure, int, (int));
};
*/

int adder_closure__callback(void* closure, int a) {
	int num = container_of(closure, struct adder_closure, closure)->num;
	return num + a;
}

void adder_closure__dtor(void* closure) {
	struct adder_closure *ctx = container_of(closure, struct adder_closure, closure);
	puts(__func__);
	free(ctx);
}

fii** make_adder_closure(int num) {
	struct adder_closure *ctx = malloc(sizeof *ctx);
	ctx->num = num;
	ctx->closure[0] = adder_closure__callback;
	ctx->closure[1] = (fii*)adder_closure__dtor;
	return ctx->closure;
}

int fun(void) {
	fii** cls = make_adder_closure(2);
	int ret = (*cls)(cls, 3);
	((closure_dtor*)cls[1])(cls);
	return ret;
}



#endif

#if 0
struct adder_closure {
	int num;
	int (*closure[2])(void *, int);
};

int adder_call(void* closure, int a) {
	struct adder_closure *ctx  = container_of(closure, struct adder_closure, closure);
	return ctx->num + a;
}

void adder_free(adder_closure *

int fun1(int (**closure)(void*, int)) {
	return (*closure)(closure, 2);
}

//int (**fun(void))(void*, int) {
int fun(void) {
	struct adder_closure capture = { .num = 3, .closure = { adder_ops } };
	return fun1(capture.closure);
}
#endif

int main() {
	printf("%d\n", fun());
}



// expands as

//struct add_capture { int a; };

/*
typedef struct{
	int (*cb)(void *, int a);
	void (*dtor)(int);
	int capture;
} add_closure_t;

static inline int add_closure_t__callback(void *capture__handle, int a) {
	int add = container_of(capture__handle, add_closure_t, cb)->capture;
	return a + add;
}

int fun1() {
	int (**closure)(void*, int) = &((add_closure_t){ .cb = add_closure_t__callback, .capture = 2 }).cb; 
	return (*(closure))((closure), 3);
}

int fun2() {
	CLOSURE_TYPE(closure, int, (int)) = CLOSURE_HEAP(add_closure_t, ({ .add = 2 }));
	int ret = CLOSURE_CALL(closure, 3);
	CLOSURE_FREE(closure);
	return ret;
}

int main() {
	printf("%d\n", fun1());
	return 0;
}
*/

#if 0


struct derivative_ctx {
	double (*fun)(double);
	double dx;
	CLOSURE_HANDLE(handle, double, (double));
};

static CLOSURE_CALLBACK(derivative_cb, double, (double x)) {
	CLOSURE_CONTEXT(struct derivative_ctx, ctx);
	double dx = ctx->dx;
	return (ctx->fun(x + dx) - ctx->fun(x - dx)) / (2.0 * dx);
}

struct derivative_ctx { double (*fun)(double); double dx; };

CLOSURE(struct derivative_ctx, double, (double x)) {
	CLOSURE_CONTEXT(ctx);
	double dx = ctx->dx;
	return (ctx->fun(x + dx) - ctx->fun(x - dx)) / (2.0 * dx);
}


double fun2(double x) {
	struct derivative_ctx ctx =  {
		.fun = fun,
		.dx = dx,
	};
	CLOSURE(closure, double, (double)) = &ctx.handle;
	return CLOSURE_CALL(closure, x);
}

#endif

#if 0
struct derivative_ctx {
	double (*closure)(void*, double);
	double (*fun)(double);
	double dx;
};

static double derivative_cb(void* closure, double x) {
	struct derivative_ctx *ctx = container_of(closure, struct derivative_ctx, closure);
	double dx = ctx->dx;
	return (ctx->fun(x + dx) - ctx->fun(x - dx)) / (2.0 * dx);
}

static struct derivative_ctx derivative_init(double (*fun)(double), double dx) {
	return (struct derivative_ctx) {
		.fun = fun,
		.dx = dx,
		.closure = derivative_cb,
	};
}

static double (**derivative_create(double (*fun)(double), double dx))(void*, double) {
	struct derivative_ctx *ctx = malloc(sizeof *ctx);
	*ctx = derivative_init(fun, dx);
	return &ctx->closure;
}

static inline double square(double x) {
	return x * x;
}

double fun(double x) {
	double (**df)(closure_t, double) = derivative_create(square, 1e-3);
	double ret = (*df)(df, x);
	free(df);
	return ret;
}


double fun2(double x) {
	struct derivative_ctx ctx = derivative_init(square, 1e-3);
	return (*(&ctx.closure))(&ctx.closure, x);
}

double fun3(double x) {
	double (*fun)(double) = square;
	double dx = 1e-3;
	double derivative(double x) {
		return (fun(x + dx) - fun(x - dx)) / (2.0 * dx);
	}
	return derivative(x);
}

int main() {
	double (**df)(closure_t, double) = derivative_create(square, 1e-3);
	double x = 3;
	printf("df(%lf) = %lf\n", x, (*df)(df, x));
	free(df);
	printf("fun: df(%lf) = %lf\n", x, fun(x));
	printf("fun2: df(%lf) = %lf\n", x, fun2(x));
	printf("fun3: df(%lf) = %lf\n", x, fun3(x));

	return 0;
}

#endif
