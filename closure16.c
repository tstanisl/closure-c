#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

typedef void * restrict closure_t;
#define CCALL__ARG1_(A,...) (A)
#define CCALL(...) ((*CCALL__ARG1_(__VA_ARGS__, ~))(__VA_ARGS__))

struct counter {
	int (*cb)(closure_t);
	int count;
};

static int counter_cb(closure_t ctx_) {
	struct counter *ctx = ctx_;
	return ctx->count++;
}

#define COUNTER() (&(struct counter) { .cb = counter_cb, .count = 0 }.cb)

typedef int fill_cb_t(closure_t);
void fill(size_t n, int data[static n], fill_cb_t **cb) {
	for (size_t i = 0; i < n; ++i)
		data[i] = CCALL(cb);
}

void print(size_t n, int A[static n]) {
	for (size_t i = 0; i < 10; ++i)
		printf("%d ", A[i]);
	puts("");
}

int main() {
	int A[10];
	fill(10, A, COUNTER());
	print(10, A);
	fill_cb_t **inc = COUNTER();
	fill(10, A, inc);
	print(10, A);
	fill(10, A, inc);
	print(10, A);
}
