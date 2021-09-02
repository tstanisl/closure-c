#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define container_of(ptr, type, member) \
  ((type*)(void*)((char*)ptr - offsetof(type, member)))

#define CLOSURE_CALL_IMPL_(args, arg0, ...) ((*(arg0)) args)
#define CLOSURE_CALL(...) CLOSURE_CALL_IMPL_((__VA_ARGS__), __VA_ARGS__, ~)

typedef void *closure_t;

typedef int fun_t(closure_t);

fun_t **make_const(int val) {
	struct capture {
		int val;
		fun_t *handle;
	};

	struct capture *cap = malloc(sizeof *cap);
	cap->val = val;
	cap->handle = ({
		int const_cb(closure_t closure) {
			struct capture *cap = container_of(closure, struct capture, handle);
			return cap->val;
		}
		const_cb;
	});
	return &cap->handle;
}

int main() {
	fun_t **closure = make_const(3);
	printf("%d\n", CLOSURE_CALL(closure));

	// memleak
	return 0;
}

