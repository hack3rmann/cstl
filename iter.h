#ifndef _CSTL_ITER_H_
#define _CSTL_ITER_H_

#include "types.h"



typedef struct Cstl_Range {
    usize start, end;
} Cstl_Range;

Cstl_Range Cstl_Range_new(usize start, usize end);

usize Cstl_Range_next(Cstl_Range mut* self);

Bool Cstl_Range_is_expired(usize const* ret);



#ifdef USING_NAMESPACE_CSTL

    typedef Cstl_Range Range;

    #define Range_new Cstl_Range_new
    #define Range_next Cstl_Range_next
    #define Range_is_expired Cstl_Range_is_expired

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_ITER_H_