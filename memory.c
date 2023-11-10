#define USING_NAMESPACE_CSTL

#include "memory.h"



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

void Cstl_mem_set(u8 mut* const ptr, u8 const value, usize const n_bytes) {
    // TODO: fast memset
    for (usize mut i = 0; i < n_bytes; ++i) {
        ptr[i] = value;
    }
}