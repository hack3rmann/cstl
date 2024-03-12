#ifndef _CSTL_MEMORY_H_
#define _CSTL_MEMORY_H_

#include "types.h"



#ifndef null
#   define null (Addr) 0
#endif

#ifndef null_mut
#   define null_mut (AddrMut) 0
#endif



AddrMut Cstl_mem_alloc(usize n_bytes);
AddrMut Cstl_mem_alloc_init(usize n_bytes, u8 init);
AddrMut Cstl_mem_alloc_zeroed(usize n_bytes);
AddrMut Cstl_mem_realloc(AddrMut ptr, usize n_bytes);
void Cstl_mem_free(AddrMut ptr);

void Cstl_mem_copy(u8 mut* dst, u8 const* src, usize n_bytes);
void Cstl_mem_move(u8 mut* dst, u8 const* src, usize n_bytes);
void Cstl_mem_set(u8 mut* ptr, u8 value, usize n_bytes);



#ifdef USING_NAMESPACE_CSTL

    #define mem_alloc Cstl_mem_alloc
    #define mem_alloc_init Cstl_mem_alloc_init
    #define mem_alloc_zeroed Cstl_mem_alloc_zeroed
    #define mem_realloc Cstl_mem_realloc
    #define mem_free Cstl_mem_free

    #define mem_copy Cstl_mem_copy
    #define mem_move Cstl_mem_move
    #define mem_set Cstl_mem_set

#endif // USING_NAMESPACE_CSTL



#endif // !_CSTL_MEMORY_H_