#ifndef _CSTL_ITERATOR_H_
#define _CSTL_ITERATOR_H_



#include "types.h"

// TODO: update `README.md`



#define Cstl_iter_next(iter) \
    _Generic( \
        iter, \
        Cstl_Range mut*: Cstl_Range_next, \
        Cstl_Range const*: Cstl_Range_next, \
        Cstl_Split mut*: Cstl_Split_next, \
        Cstl_Split const*: Cstl_Split_next, \
        Cstl_SplitAny mut*: Cstl_SplitAny_next, \
        Cstl_SplitAny const*: Cstl_SplitAny_next, \
        Cstl_SplitWhitespace mut*: Cstl_SplitWhitespace_next, \
        Cstl_SplitWhitespace const*: Cstl_SplitWhitespace_next, \
        Cstl_Chars mut*: Cstl_Chars_next, \
        Cstl_Chars const*: Cstl_Chars_next \
    )(iter)

#define Cstl_iter_is_expired(iter, ret) \
    _Generic( \
        iter, \
        Cstl_Range mut*: Cstl_Range_is_expired, \
        Cstl_Range const*: Cstl_Range_is_expired, \
        Cstl_Split mut*: Cstl_Split_is_expired, \
        Cstl_Split const*: Cstl_Split_is_expired, \
        Cstl_SplitAny mut*: Cstl_SplitAny_is_expired, \
        Cstl_SplitAny const*: Cstl_SplitAny_is_expired, \
        Cstl_SplitWhitespace mut*: Cstl_SplitWhitespace_is_expired, \
        Cstl_SplitWhitespace const*: Cstl_SplitWhitespace_is_expired, \
        Cstl_Chars mut*: Cstl_Chars_is_expired, \
        Cstl_Chars const*: Cstl_Chars_is_expired \
    )(ret)

#define Cstl_iter_foreach(iter, next, block) \
    { \
        typeof(Cstl_iter_next(&mut iter)) mut next; \
        while (True) { \
            next = Cstl_iter_next(&mut iter); \
            if (Cstl_iter_is_expired(&iter, &next)) { \
                break; \
            } \
            block \
        } \
    }



typedef struct Cstl_Range {
    usize start, end;
} Cstl_Range;

Cstl_Range Cstl_Range_new(usize start, usize end);

usize Cstl_Range_next(Cstl_Range mut* self);

Bool Cstl_Range_is_expired(usize const* ret);



#ifdef USING_NAMESPACE_CSTL

    #define iter_next Cstl_iter_next
    #define iter_is_expired Cstl_iter_is_expired
    #define iter_foreach Cstl_iter_foreach

    typedef Cstl_Range Range;

    #define Range_new Cstl_Range_new
    #define Range_next Cstl_Range_next
    #define Range_is_expired Cstl_Range_is_expired

#endif



#endif // !_CSTL_ITERATOR_H_