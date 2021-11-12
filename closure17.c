#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

typedef void* closure_t;
#define CCALL__ARG1_(A,...) (A)
#define CCALL(...) ((*CCALL__ARG1_(__VA_ARGS__, ~))(__VA_ARGS__))

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
	struct counter {
		int (*cb)(closure_t);
		int count;
	} counter = {
		.cb = ({int _fn(closure_t ctx) { return ((struct counter*)ctx)->count++; } _fn; }),
		.count = 0,
	};
	int A[10];
	fill(10, A, &counter.cb);
	print(10, A);
	fill(10, A, &counter.cb);
	print(10, A);
	fill(10, A, &counter.cb);
	print(10, A);
}
