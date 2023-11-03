#ifndef _CSTL_UTIL_H_
#define _CSTL_UTIL_H_

#include "types.h"



#define Cstl_array_len(array) (sizeof((array)) / sizeof(*(array)))

#define Cstl_swap_values(lhs_ptr, rhs_ptr) \
    Cstl_swap((lhs_ptr), (rhs_ptr), sizeof(*lhs_ptr)) 

#define mut

#define Cstl_lit_ptr(Type, value) \
    ((Type[]) { (Type) (value) })



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

    #define CollectionMeta_elem_size(self) \
        Cstl_CollectionMeta_elem_size(self)



    #define array_len(array) \
        Cstl_array_len(array)

    #define lit_ptr(Type, value) \
        Cstl_lit_ptr(Type, value)

    #define swap_values(lhs_ptr, rhs_ptr) \
        Cstl_swap_values(lhs_ptr, rhs_ptr)

    #define swap(lhs, rhs, n_bytes) \
        Cstl_swap(lhs, rhs, n_bytes)

    #define swap_8(lhs, rhs) \
        Cstl_swap_8(lhs, rhs)
    
    #define swap_16(lhs, rhs) \
        Cstl_swap_16(lhs, rhs)

    #define swap_32(lhs, rhs) \
        Cstl_swap_32(lhs, rhs)

    #define swap_64(lhs, rhs) \
        Cstl_swap_64(lhs, rhs)

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_UTIL_H_