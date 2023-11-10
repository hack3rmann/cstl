#define USING_NAMESPACE_CSTL

#include "compare.h"

#define f32_EPS 1e-6f
#define f64_EPS 1e-12



#define IMPLEMENT_CMP(Type) \
    Cstl_Ordering Cstl_ ## Type ## _cmp( \
        Addr lhs_ptr, Addr rhs_ptr \
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
IMPLEMENT_CMP(char);
IMPLEMENT_CMP(Bool);

Cstl_Ordering Cstl_f32_cmp(Addr lhs_ptr, Addr rhs_ptr) {
    f32 const lhs = *(f32 const*) lhs_ptr;
    f32 const rhs = *(f32 const*) rhs_ptr;
    
    if (lhs + f32_EPS < rhs) {
        return Cstl_Ordering_Less;
    } else if (rhs + f32_EPS < lhs) {
        return Cstl_Ordering_Greater;
    } else {
        return Cstl_Ordering_Equal;
    }
}

Cstl_Ordering Cstl_f64_cmp(Addr lhs_ptr, Addr rhs_ptr) {
    f64 const lhs = *(f64 const*) lhs_ptr;
    f64 const rhs = *(f64 const*) rhs_ptr;
    
    if (lhs + f64_EPS < rhs) {
        return Cstl_Ordering_Less;
    } else if (rhs + f64_EPS < lhs) {
        return Cstl_Ordering_Greater;
    } else {
        return Cstl_Ordering_Equal;
    }
}

Cstl_Ordering Cstl_Ordering_reverse(Cstl_Ordering const self) {
    return -self;
}
