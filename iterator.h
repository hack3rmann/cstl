#ifndef _CSTL_ITERATOR_H_
#define _CSTL_ITERATOR_H_



#include "types.h"



#define Cstl_iterate(IterType, iter, next, tt) \
    { \
        typeof(IterType ## _next(&mut iter)) mut next; \
        \
        while (True) { \
            next = IterType ## _next(&mut iter); \
            \
            if (IterType ## _is_expired(&next)) { \
                break; \
            } \
            \
            tt \
        } \
    }



typedef struct Cstl_Range {
    usize start, end;
} Cstl_Range;

Cstl_Range Cstl_Range_new(usize start, usize end);

usize Cstl_Range_next(Cstl_Range mut* self);

Bool Cstl_Range_is_expired(usize const* ret);



#ifdef USING_NAMESPACE_CSTL

    #define iterate Cstl_iterate

    typedef Cstl_Range Range;

    #define Range_new Cstl_Range_new
    #define Range_next Cstl_Range_next
    #define Range_is_expired Cstl_Range_is_expired

#endif



#endif // !_CSTL_ITERATOR_H_