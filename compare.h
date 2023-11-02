#ifndef _CSTL_COMPARE_H_
#define _CSTL_COMPARE_H_

#include "types.h"



typedef enum {
    Cstl_Ordering_Less = -1,
    Cstl_Ordering_Equal = 0,
    Cstl_Ordering_Greater = 1
} Cstl_Ordering;

typedef Cstl_Ordering (*Cstl_Comparator)(void const*, void const*);

Cstl_Ordering Cstl_Ordering_reverse(Cstl_Ordering self);

Cstl_Ordering Cstl_u8_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_i8_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_u16_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_i16_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_u32_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_i32_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_u64_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_i64_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_usize_cmp(void const* lhs, void const* rhs);
Cstl_Ordering Cstl_isize_cmp(void const* lhs, void const* rhs);

u8 u8_max(u8 lhs, u8 rhs);
i8 i8_max(i8 lhs, i8 rhs);
u16 u16_max(u16 lhs, u16 rhs);
i16 i16_max(i16 lhs, i16 rhs);
u32 u32_max(u32 lhs, u32 rhs);
i32 i32_max(i32 lhs, i32 rhs);
u64 u64_max(u64 lhs, u64 rhs);
i64 i64_max(i64 lhs, i64 rhs);
usize usize_max(usize lhs, usize rhs);
isize isize_max(isize lhs, isize rhs);

u8 u8_min(u8 lhs, u8 rhs);
i8 i8_min(i8 lhs, i8 rhs);
u16 u16_min(u16 lhs, u16 rhs);
i16 i16_min(i16 lhs, i16 rhs);
u32 u32_min(u32 lhs, u32 rhs);
i32 i32_min(i32 lhs, i32 rhs);
u64 u64_min(u64 lhs, u64 rhs);
i64 i64_min(i64 lhs, i64 rhs);
usize usize_min(usize lhs, usize rhs);
isize isize_min(isize lhs, isize rhs);



#ifdef USING_NAMESPACE_CSTL

    typedef Cstl_Ordering Ordering;

    #define Ordering_Less Cstl_Ordering_Less
    #define Ordering_Equal Cstl_Ordering_Equal
    #define Ordering_Greater Cstl_Ordering_Greater

    Ordering Ordering_reverse(Ordering self);

    Ordering u8_cmp(void const* lhs, void const* rhs);
    Ordering i8_cmp(void const* lhs, void const* rhs);
    Ordering u16_cmp(void const* lhs, void const* rhs);
    Ordering i16_cmp(void const* lhs, void const* rhs);
    Ordering u32_cmp(void const* lhs, void const* rhs);
    Ordering i32_cmp(void const* lhs, void const* rhs);
    Ordering u64_cmp(void const* lhs, void const* rhs);
    Ordering i64_cmp(void const* lhs, void const* rhs);
    Ordering usize_cmp(void const* lhs, void const* rhs);
    Ordering isize_cmp(void const* lhs, void const* rhs);

#endif

#endif