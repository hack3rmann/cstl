#define USING_NAMESPACE_CSTL

#include "compare.h"


#define IMPLEMENT_CMP(Type) \
    Cstl_Ordering Cstl_ ## Type ## _cmp( \
        const void* lhs_ptr, const void* rhs_ptr \
    ) { \
        Type const lhs = *(Type*)lhs_ptr; \
        Type const rhs = *(Type*)rhs_ptr; \
        if (lhs < rhs) { \
            return Cstl_Ordering_Less; \
        } else if (lhs == rhs) { \
            return Cstl_Ordering_Equal; \
        } else { \
            return Cstl_Ordering_Greater; \
        } \
    } \
    \
    Cstl_Ordering Type ## _cmp( \
        const void* lhs_ptr, const void* rhs_ptr \
    ) { \
        return Cstl_ ## Type ## _cmp(lhs_ptr, rhs_ptr); \
    } \
    \
    Type Type ## _max(Type const lhs, Type const rhs) { \
        return lhs ^ ((lhs ^ rhs) & -(lhs < rhs)); \
    } \
    \
    Type Type ## _min(Type const lhs, Type const rhs) { \
        return rhs ^ ((lhs ^ rhs) & -(lhs < rhs)); \
    }

IMPLEMENT_CMP(u8)
IMPLEMENT_CMP(i8)
IMPLEMENT_CMP(u16)
IMPLEMENT_CMP(i16)
IMPLEMENT_CMP(u32)
IMPLEMENT_CMP(i32)
IMPLEMENT_CMP(u64)
IMPLEMENT_CMP(i64)
IMPLEMENT_CMP(usize)
IMPLEMENT_CMP(isize)

Cstl_Ordering Cstl_Ordering_reverse(Cstl_Ordering const self) {
    return -self;
}
