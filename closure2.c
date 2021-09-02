#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

#define CLOSURE_CALL_IMPL_(args, arg0, ...) ((*(arg0)) args)
#define CLOSURE_CALL(...) CLOSURE_CALL_IMPL_((__VA_ARGS__), __VA_ARGS__, ~)

//typedef struct { void *ptr; } closure_t;
typedef void *closure_t;

typedef int fun_t(closure_t);

struct const_capture {
	int val;
	fun_t *handle;
};

static int const_cb(closure_t closure) {
	struct const_capture *ctx = container_of(closure, struct const_capture, handle);
	return ctx->val;
}

fun_t **make_const(int val) {
	struct const_capture *ctx = malloc(sizeof *ctx);
	ctx->val = val;
	ctx->handle = const_cb;
	return &ctx->handle;
}

int main() {
	fun_t **closure = make_const(3);
	printf("%d\n", CLOSURE_CALL(closure));

	// memleak
	return 0;
}

