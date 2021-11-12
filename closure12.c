#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

#define CLOSURE_CALL_IMPL_(args, arg0, ...) ((*(arg0)) args)
#define CLOSURE_CALL(...) CLOSURE_CALL_IMPL_((__VA_ARGS__), __VA_ARGS__, ~)

#define const
typedef const void *closure_t;

typedef int fun_t(closure_t, int);

struct adder_closure {
	int val;
	fun_t * const closure;
} adder_closure(int val) {
	int cb(closure_t closure, int x) {
		struct adder_closure *cap = container_of(closure, struct adder_closure, closure);
		return x + cap->val;
	}
	return (struct adder_closure) { .val = val, .closure = cb };
}

int foo(void) {
	struct adder_closure closure = adder_closure(3);
	return CLOSURE_CALL(&closure.closure, 2);
}

int main() {
	struct adder_closure closure = adder_closure(5);
	fun_t * const * f = &closure.closure;
	for (int i = 0; i < 5; ++i) {
		printf("%d\n", CLOSURE_CALL(f, i));
	}
	return 0;
}


