#if 0
#define SECOND(...) SECOND_I(__VA_ARGS__,,)
#define SECOND_I(A,B,...) B
#endif
#define ONE_OR_MANY_UTILITY_I(A,B,...) B
#define SHIFT_IN_ZERO , 0
#define ONE_OR_MANY_UTILITY(...) ONE_OR_MANY_UTILITY_I(__VA_ARGS__, SHIFT_IN_ZERO, X)
#define ONE_OR_MANY(...) SECOND(ONE_OR_MANY_UTILITY(__VA_ARGS__), 1)


ONE_OR_MANY(A)

//ONE_OR_MANY(A,B)

