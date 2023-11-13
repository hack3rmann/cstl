#define USING_NAMESPACE_CSTL

#include "memory.h"



Addr Addr_align_up(Addr const self, usize const n_bytes) {
    return (u8 mut*) Addr_align_down(self, n_bytes) + n_bytes;
}

Addr Addr_align_down(Addr const self, usize const n_bytes) {
    usize mut address = Addr_as_usize(self);

    address &= ~(n_bytes - 1);

    return Addr_from_usize(address);
}

AddrMut AddrMut_align_up(AddrMut const self, usize const n_bytes) {
    return (u8 mut*) AddrMut_align_down(self, n_bytes) + n_bytes;
}

AddrMut AddrMut_align_down(AddrMut const self, usize const n_bytes) {
    usize mut address = Addr_as_usize(self);

    address &= ~(n_bytes - 1);

    return AddrMut_from_usize(address);
}



// TODO: implement brand new allocator for `cstl`

extern AddrMut malloc(usize n_bytes);
extern AddrMut calloc(usize n_elems, usize elem_size);
extern AddrMut realloc(AddrMut mem_ptrl, usize n_bytes);
extern void free(AddrMut mem_ptr);
extern AddrMut memcpy(AddrMut dst, Addr src, usize n_bytes);
extern AddrMut memmove(AddrMut dst, Addr src, usize n_bytes);



AddrMut Cstl_mem_alloc(usize const n_bytes) {
    return malloc(n_bytes);
}

AddrMut Cstl_mem_alloc_init(usize const n_bytes, u8 const init) {
    AddrMut const result = Cstl_mem_alloc(n_bytes);

    Cstl_mem_set((u8 mut*) result, init, n_bytes);

    return result;
}

AddrMut Cstl_mem_alloc_zeroed(usize const n_bytes) {
    return calloc(n_bytes, sizeof(u8));
}

AddrMut Cstl_mem_realloc(AddrMut const ptr, usize const n_bytes) {
    return realloc(ptr, n_bytes);
}

void Cstl_mem_free(AddrMut const ptr) {
    free(ptr);
}

void Cstl_mem_copy(u8 mut* const dst, u8 const* const src, usize const n_bytes) {
    memcpy(dst, src, n_bytes);
}

void Cstl_mem_move(u8 mut* const dst, u8 const* const src, usize const n_bytes) {
    memmove(dst, src, n_bytes);
}

void Cstl_mem_set(u8 mut* mut ptr, u8 const value, usize mut n_bytes) {
    if (n_bytes < 8) {
        for (usize mut i = 0; i < n_bytes; ++i) {
            ptr[i] = value;
        }

        return;
    }

    // the value that bytewise holds `value`
    usize mut large_value = 0;

    for (usize mut i = 0; i < sizeof(usize); ++i) {
        large_value |= (usize) value << i;
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