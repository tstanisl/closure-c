#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

#define CLOSURE_CALL_IMPL_(args, arg0, ...) ((*(arg0)) args)
#define CLOSURE_CALL(...) CLOSURE_CALL_IMPL_((__VA_ARGS__), __VA_ARGS__, ~)

typedef void *closure_t;

typedef bool filter_t(closure_t, int);

int do_filter(int n, int *arr, filter_t **closure) {
	int w = 0;
	for (int r = 0; r < n; ++r)
		if (CLOSURE_CALL(closure, arr[r]))
			arr[w++] = arr[r];
	return w;
}

int main() {
	int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	int val;
	scanf("%d", &val);
	filter_t **lambda = &((struct cap { int val; filter_t *cb; })
		{ .val = val, .cb = ({
			bool cb(closure_t closure, int val) {
				struct cap *cap = container_of(closure, struct cap, cb);
				return val % cap->val == 0;
			} cb; })}).cb;
	int n = 1[&a]-a;
	n = do_filter(n, a, lambda);
	for (int i = 0; i < n; ++i)
		printf("%d ", a[i]);
	puts("");
	return 0;
}

