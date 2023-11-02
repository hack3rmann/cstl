#ifndef _CSTL_ERROR_H_
#define _CSTL_ERROR_H_

#define Cstl_deny_fmt(fmt, args...) \
    do { \
        fprintf( \
            stderr, \
            "Cstl_deny called in file `%s` in function `%s` on line `%d`:\n", \
            __FILE__, __FUNCTION__, __LINE__ \
        ); \
        fprintf(stderr, fmt "\n", args); \
        abort(); \
    } while (0)

#define Cstl_deny(reason) \
    Cstl_deny_fmt(reason "%s", "")

#define Cstl_assert_fmt(expr, fmt, args...) \
    do { \
        if (!(expr)) { \
            Cstl_deny_fmt("assertion failed: `%s` " fmt, #expr, args); \
        } \
    } while (0)

#define Cstl_assert_msg(expr, msg) \
    Cstl_assert_fmt(expr, msg "%s", "")

#define Cstl_assert(expr) \
    Cstl_assert_msg(expr, "")

#define Cstl_todo(plan) \
    Cstl_deny("not yet unimplemented. " plan)

#define Cstl_unreachable_msg(reason) \
    Cstl_deny("unreachable by argument: \'" reason "\' branch had reached")

#define Cstl_unreachable() \
    Cstl_deny("unreachable branch had reached")


#endif // !_CSTL_ERROR_H