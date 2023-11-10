#ifndef _CSTL_UTIL_H_
#define _CSTL_UTIL_H_

#include "types.h"



#define Cstl_array_len(array) (sizeof((array)) / sizeof(*(array)))

#define Cstl_swap_values(lhs_ptr, rhs_ptr) \
    Cstl_swap((lhs_ptr), (rhs_ptr), sizeof(*lhs_ptr)) 

#define Cstl_lit_ptr(Type, value) \
    ((Type[]) { (Type) (value) })

#define UNUSED __attribute__((unused))



typedef usize Cstl_CollectionMeta;

usize Cstl_CollectionMeta_elem_size(Cstl_CollectionMeta self);

usize elem_size_of(Cstl_CollectionMeta meta);



void Cstl_swap(AddrMut lhs, AddrMut rhs, usize n_bytes);

void Cstl_swap_8(AddrMut lhs, AddrMut rhs);
void Cstl_swap_16(AddrMut lhs, AddrMut rhs);
void Cstl_swap_32(AddrMut lhs, AddrMut rhs);
void Cstl_swap_64(AddrMut lhs, AddrMut rhs);



#ifdef USING_NAMESPACE_CSTL

    typedef Cstl_CollectionMeta CollectionMeta;

    #define CollectionMeta_elem_size Cstl_CollectionMeta_elem_size



    #define array_len Cstl_array_len
    #define lit_ptr Cstl_lit_ptr
    #define swap_values Cstl_swap_values
    #define swap Cstl_swap
    #define swap_8 Cstl_swap_8
    #define swap_16 Cstl_swap_16
    #define swap_32 Cstl_swap_32
    #define swap_64 Cstl_swap_64

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_UTIL_H_