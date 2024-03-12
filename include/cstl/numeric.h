#ifndef _CSTL_NUMERIC_H_
#define _CSTL_NUMERIC_H_



#include "types.h"



i8 i8_abs(i8 self);
i16 i16_abs(i16 self);
i32 i32_abs(i32 self);
i64 i64_abs(i64 self);
isize isize_abs(isize self);

#define abs(value) \
    _Generic( \
        value, \
        i8: i8_abs, \
        i16: i16_abs, \
        i32: i32_abs, \
        i64: i64_abs \
    )(value)



#endif // !_CSTL_NUMERIC_H_