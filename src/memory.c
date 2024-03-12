#define USING_NAMESPACE_CSTL

#include "../include/cstl/memory.h"
#include "../include/cstl/compare.h"



Addr Addr_align_up(Addr const self, usize const n_bytes) {
    return Addr_align_down((u8 mut*) self + n_bytes - 1, n_bytes);
}

Addr Addr_align_down(Addr const self, usize const n_bytes) {
    usize mut address = Addr_as_usize(self);

    address &= ~(n_bytes - 1);

    return Addr_from_usize(address);
}

AddrMut AddrMut_align_up(AddrMut const self, usize const n_bytes) {
    return AddrMut_align_down((u8 mut*) self + n_bytes - 1, n_bytes);
}

AddrMut AddrMut_align_down(AddrMut const self, usize const n_bytes) {
    usize mut address = Addr_as_usize(self);

    address &= ~(n_bytes - 1);

    return AddrMut_from_usize(address);
}

usize Cstl_mem_align_size(usize const n_bytes) {
    if (n_bytes <= sizeof(Addr)) {
#       if Addr_SIZE == 2
            return n_bytes
#       elif Addr_SIZE == 4
            if (n_bytes <= 2) {
                return n_bytes;
            } else {
                return sizeof(Addr);
            }
#       elif Addr_SIZE == 8
            if (n_bytes <= 2) {
                return n_bytes;
            } else if (n_bytes <= 4) {
                return 4;
            } else {
                return sizeof(Addr);
            }
#       endif
    } else {
        usize mut result = 2 * sizeof(Addr);

        while (result < n_bytes) {
            result += sizeof(Addr);
        }

        return result;
    }
}



extern AddrMut malloc(usize n_bytes);
extern AddrMut calloc(usize n_elems, usize elem_size);
extern AddrMut realloc(AddrMut mem_ptr, usize n_bytes);
extern void free(AddrMut mem_ptr);
extern AddrMut memcpy(AddrMut dst, Addr src, usize n_bytes);
extern AddrMut memmove(AddrMut dst, Addr src, usize n_bytes);



AddrMut Cstl_mem_alloc_unaligned(usize const n_bytes) {
    return malloc(n_bytes);
}

AddrMut Cstl_mem_alloc_unaligned_init(usize const n_bytes, u8 const init) {
    AddrMut const result = Cstl_mem_alloc_unaligned(n_bytes);

    Cstl_mem_set((u8 mut*) result, init, n_bytes);

    return result;
}

AddrMut Cstl_mem_alloc_unaligned_zeroed(usize const n_bytes) {
    return calloc(n_bytes, sizeof(u8));
}



AddrMut Cstl_mem_alloc(usize const n_elems, usize mut elem_size) {
    // align element size
    elem_size = mem_align_size(elem_size);

    usize const cell_size = usize_min(sizeof(Addr), elem_size);

    AddrMut const unaligned_ptr
        = Cstl_mem_alloc_unaligned(n_elems * elem_size + cell_size);

    AddrMut const aligned_ptr = AddrMut_align_up(unaligned_ptr, cell_size);

    u8 const n_align_bytes
        = (u8) ((u8 mut*) aligned_ptr - (u8 mut*) unaligned_ptr);

    *((u8 mut*) aligned_ptr + n_elems * elem_size) = n_align_bytes;

    return aligned_ptr;
}

AddrMut Cstl_mem_alloc_init(
    usize const n_elems, usize const elem_size, u8 const value
) {
    AddrMut const result = Cstl_mem_alloc(n_elems, elem_size);

    Cstl_mem_set((u8 mut*) result, value, n_elems * mem_align_size(elem_size));

    return result;
}

AddrMut Cstl_mem_alloc_zeroed(usize const n_elems, usize const elem_size) {
    return Cstl_mem_alloc_init(n_elems, elem_size, 0);
}

AddrMut Cstl_mem_realloc(
    AddrMut mut ptr, usize const old_n_elems, usize mut elem_size,
    usize const n_elems
) {
    elem_size = mem_align_size(elem_size);
    usize const cell_size = usize_min(sizeof(Addr), elem_size);
    
    usize mut n_align_bytes = *((u8 mut*) ptr + old_n_elems * elem_size);

    ptr = (u8 mut*) ptr - n_align_bytes;

    ptr = Cstl_mem_realloc_unaligned(ptr, elem_size * n_elems);

    AddrMut const aligned_ptr = AddrMut_align_up(ptr, cell_size);

    n_align_bytes = (u8 mut*) aligned_ptr - (u8 mut*) ptr;
    *((u8 mut*) aligned_ptr + n_elems * elem_size) = (u8) n_align_bytes;

    return aligned_ptr;
}

void Cstl_mem_free(
    AddrMut const ptr, usize const n_elems, usize mut elem_size
) {
    elem_size = mem_align_size(elem_size);

    // assume `ptr` is aligned
    usize const n_align_bytes = *((u8 mut*) ptr + n_elems * elem_size);

    AddrMut const unaligned_ptr = (u8 mut*) ptr - n_align_bytes;

    Cstl_mem_free_unaligned(unaligned_ptr);
}



AddrMut Cstl_mem_realloc_unaligned(AddrMut const ptr, usize const n_bytes) {
    return realloc(ptr, n_bytes);
}

void Cstl_mem_free_unaligned(AddrMut const ptr) {
    free(ptr);
}

void Cstl_mem_copy(
    u8 mut* const dst, u8 const* const src, usize const n_bytes
) {
    memcpy(dst, src, n_bytes);
}

void Cstl_mem_move(
    u8 mut* const dst, u8 const* const src, usize const n_bytes
) {
    memmove(dst, src, n_bytes);
}

void Cstl_mem_set(u8 mut* mut ptr, u8 const value, usize mut n_bytes) {
    if (n_bytes < 8) {
        for (usize mut i = 0; i < n_bytes; ++i) {
            ptr[i] = value;
        }

        return;
    }

    // all bytes of `large_value` will be `value`
    usize mut large_value = 0;

    for (usize mut i = 0; i < sizeof(usize); ++i) {
        large_value |= (usize) value << 8 * i;
    }

    usize mut* const large_pointer = AddrMut_align_up(ptr, sizeof(Addr));

    for (usize mut i = 0; i < n_bytes / sizeof(Addr); ++i) {
        large_pointer[i] = large_value;
    }
    
    // skip all vectorized values
    ptr += n_bytes - (n_bytes & (sizeof(Addr) - 1));
    n_bytes -= n_bytes & ~(sizeof(Addr) - 1);

    for (usize mut i = 0; i < n_bytes; ++i) {
        ptr[i] = value;
    }
}

void Cstl_mem_set_aligned(
    u8 mut* const ptr, u8 const value, usize const n_bytes
) {
    usize mut large_value = 0;

    for (usize mut i = 0; i < sizeof(Addr); ++i) {
        large_value |= (usize) value << 8 * i;
    }

    for (usize mut i = 0; i < n_bytes / sizeof(Addr); ++i) {
        ((usize mut*) ptr)[i] = large_value;
    }
}