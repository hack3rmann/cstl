#ifndef _CSTL_ITER_H_
#define _CSTL_ITER_H_



#include "types.h"



typedef struct {
    usize start, end;
} Cstl_Range;

Cstl_Range Cstl_Range_new(usize start, usize end);

usize Cstl_Range_next(Cstl_Range mut* self);

Bool Cstl_Range_is_expired(usize const* ret);



#ifdef USING_NAMESPACE_CSTL

    typedef Cstl_Range Range;

    #define Range_new(start, end) \
        Cstl_Range_new(start, end)

    #define Range_next(self) \
        Cstl_Range_next(self) 

    #define Range_is_expired(ret) \
        Cstl_Range_is_expired(ret)

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_ITER_H_