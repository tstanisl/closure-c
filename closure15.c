typedef int fun_t(int);

static int square(int x) { return x * x; }

int foo(fun_t **f) { return (*f)(2); }
int bar(fun_t **f) { return (*f)(2) + (*f)(3); }

int fun(void) {
	fun_t *fun = square;
	//return foo(&fun);
	return bar(&fun);
}
