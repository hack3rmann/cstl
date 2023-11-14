#ifndef _CSTL_MEMORY_H_
#define _CSTL_MEMORY_H_

#include "types.h"



#define null (Addr) 0
#define null_mut (AddrMut) 0



/// # Safety
/// 
/// `n_bytes` should be a power of 2
Addr Addr_align_up(Addr self, usize n_bytes);

/// # Safety
/// 
/// `n_bytes` should be a power of 2
Addr Addr_align_down(Addr self, usize n_bytes);

/// # Safety
/// 
/// `n_bytes` should be a power of 2
AddrMut AddrMut_align_up(AddrMut self, usize n_bytes);

/// # Safety
/// 
/// `n_bytes` should be a power of 2
AddrMut AddrMut_align_down(AddrMut self, usize n_bytes);

usize Cstl_mem_align_size(usize n_bytes);



AddrMut Cstl_mem_alloc_unaligned(usize n_bytes);
AddrMut Cstl_mem_alloc_unaligned_init(usize n_bytes, u8 init);
AddrMut Cstl_mem_alloc_unaligned_zeroed(usize n_bytes);

AddrMut Cstl_mem_alloc(usize n_elems, usize elem_size);
AddrMut Cstl_mem_alloc_init(usize n_elems, usize elem_size, u8 value);
AddrMut Cstl_mem_alloc_zeroed(usize n_elems, usize elem_size);

AddrMut Cstl_mem_realloc_unaligned(AddrMut ptr, usize n_bytes);
AddrMut Cstl_mem_realloc(
    AddrMut ptr, usize old_n_elems, usize elem_size, usize n_elems
);

void Cstl_mem_free_unaligned(AddrMut ptr);
void Cstl_mem_free(AddrMut ptr, usize n_elems, usize elem_size);

void Cstl_mem_copy(u8 mut* dst, u8 const* src, usize n_bytes);
void Cstl_mem_move(u8 mut* dst, u8 const* src, usize n_bytes);
void Cstl_mem_set(u8 mut* ptr, u8 value, usize n_bytes);



#ifdef USING_NAMESPACE_CSTL

    #define mem_align_size Cstl_mem_align_size

    #define mem_alloc_unaligned Cstl_mem_alloc_unaligned
    #define mem_alloc_unaligned_init Cstl_mem_alloc_unaligned_init
    #define mem_alloc_unaligned_zeroed Cstl_mem_alloc_unaligned_zeroed

    #define mem_alloc Cstl_mem_alloc
    #define mem_alloc_init Cstl_mem_alloc_init
    #define mem_alloc_zeroed Cstl_mem_alloc_zeroed

    #define mem_realloc_unaligned Cstl_mem_realloc_unaligned
    #define mem_realloc Cstl_mem_realloc

    #define mem_free_unaligned Cstl_mem_free_unaligned
    #define mem_free Cstl_mem_free

    #define mem_copy Cstl_mem_copy
    #define mem_move Cstl_mem_move
    #define mem_set Cstl_mem_set

#endif // USING_NAMESPACE_CSTL



#endif // !_CSTL_MEMORY_H_