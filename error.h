#ifndef _CSTL_ERROR_H_
#define _CSTL_ERROR_H_

#include "io.h"
#include "string.h"


extern void abort(void);
extern void exit(int exit_code);



#define Cstl_deny_fmt(fmt, args...) \
    do { \
        Cstl_println( \
            "deny called in function `{str}` in file `{str}` on line `{usize}`:", \
            Cstl_str_from_literal(__FUNCTION__), \
            Cstl_str_from_literal(__FILE__), \
            (usize) (__LINE__) \
        ); \
        Cstl_println(fmt, args); \
        abort(); \
    } while (0)

#define Cstl_deny(reason) \
    Cstl_deny_fmt(reason "{str}", Cstl_str_from_literal(""))

#define Cstl_assert_fmt(expr, fmt, args...) \
    do { \
        if (!(expr)) { \
            Cstl_deny_fmt( \
                "assertion failed: `{str}` " fmt, \
                Cstl_str_from_literal(#expr), \
                args \
            ); \
        } \
    } while (0)

#define Cstl_assert_msg(expr, msg) \
    Cstl_assert_fmt(expr, "{str}", Cstl_str_from_literal(msg))

#define Cstl_assert(expr) \
    Cstl_assert_msg(expr, "")

#define Cstl_todo(plan) \
    Cstl_deny_fmt("not yet unimplemented. {str}", Cstl_str_from_literal(plan))

#define Cstl_unreachable_msg(reason) \
    Cstl_deny_fmt( \
        "unreachable by argument: {str} branch had reached", \
        Cstl_str_from_literal(reason) \
    )

#define Cstl_unreachable() \
    Cstl_deny("unreachable branch had reached")



#ifdef USING_NAMESPACE_CSTL

    #define deny_fmt Cstl_deny_fmt
    #define deny Cstl_deny
    #define assert_fmt Cstl_assert_fmt
    #define assert_msg Cstl_assert_msg
    #define assert Cstl_assert
    #define todo Cstl_todo
    #define unreachable_msg Cstl_unreachable_msg
    #define unreachable Cstl_unreachable

#endif // USING_NAMESPACE_CSTL



#endif // !_CSTL_ERROR_H