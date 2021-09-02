#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

#define CLOSURE_CALL_IMPL_(args, arg0, ...) ((*(arg0)) args)
#define CLOSURE_CALL(...) CLOSURE_CALL_IMPL_((__VA_ARGS__), __VA_ARGS__, ~)

typedef void *closure_t;

typedef int fun_t(closure_t);

struct const_capture {
	int val;
	fun_t *handle;
} const_capture(int val) {
	int cb(closure_t closure) {
		struct const_capture *cap = container_of(closure, struct const_capture, handle);
		return cap->val;
	}
	return (struct const_capture) { .val = val, .handle = cb, };
}

int main() {
	struct const_capture cap = const_capture(3);
	fun_t **closure = &cap.handle;
	printf("%d\n", CLOSURE_CALL(closure));
	return 0;
}

