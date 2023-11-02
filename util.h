#ifndef _CSTL_UTIL_H_
#define _CSTL_UTIL_H_

#include "types.h"



#define Cstl_array_len(array) (sizeof((array)) / sizeof(*(array)))

#define Cstl_swap_values(lhs_ptr, rhs_ptr) \
    Cstl_swap((lhs_ptr), (rhs_ptr), sizeof(*lhs_ptr)) 

#define mut

#define Cstl_lit_ptr(Type, value) \
    ((Type[]) { (Type) (value) })



typedef union {
    usize elem_size;
    struct {
        u8 _padding[sizeof(usize) - 2];
        u16 meta;
    } last_byte;
} Cstl_CollectionMetaData;

usize Cstl_CollectionMetaData_elem_size(Cstl_CollectionMetaData meta);



void Cstl_swap(AddrMut lhs, AddrMut rhs, usize n_bytes);

void Cstl_swap_8(AddrMut lhs, AddrMut rhs);
void Cstl_swap_16(AddrMut lhs, AddrMut rhs);
void Cstl_swap_32(AddrMut lhs, AddrMut rhs);
void Cstl_swap_64(AddrMut lhs, AddrMut rhs);



#ifdef USING_NAMESPACE_CSTL

    #define array_len(array) Cstl_array_len(array)

    #define lit_ptr(Type, value) \
        Cstl_lit_ptr(Type, value)

    #define swap_values(lhs_ptr, rhs_ptr) Cstl_swap_values(lhs_ptr, rhs_ptr)

    void swap(AddrMut lhs, AddrMut rhs, usize n_bytes);

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_UTIL_H_