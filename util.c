#define USING_NAMESPACE_CSTL

#include <string.h>
#include <stdlib.h>

#include "util.h"



usize Cstl_CollectionMeta_elem_size(Cstl_CollectionMeta const self) {
    return self & u16_MAX;
}

usize elem_size_of(Cstl_CollectionMeta const meta) {
    return Cstl_CollectionMeta_elem_size(meta);
}

void Cstl_swap(AddrMut const lhs, AddrMut const rhs, usize const n_bytes) {
    if (lhs == rhs || 0 == n_bytes) {
        return;
    }

    switch (n_bytes) {
    case 1:
        Cstl_swap_8(lhs, rhs);
        return;
    case 2:
        Cstl_swap_16(lhs, rhs);
        return;
    case 4:
        Cstl_swap_32(lhs, rhs);
        return;
    case 8:
        Cstl_swap_64(lhs, rhs);
        return;

    default: {
        enum { BUFFER_LEN = 64 };

        static u8 static_buffer[BUFFER_LEN];

        Bool const is_allocated = BUFFER_LEN < n_bytes;

        u8 mut* buffer = is_allocated ? malloc(n_bytes) : static_buffer;

        memcpy(buffer, rhs, n_bytes);
        memcpy(rhs, lhs, n_bytes);
        memcpy(lhs, buffer, n_bytes);

        if (is_allocated) {
            free(buffer);
        }
    } break;
    }
}

void Cstl_swap_8(AddrMut const lhs, AddrMut const rhs) {
    u8 const tmp = *(u8 const*) lhs;
    *(u8 mut*) lhs = *(u8 const*) rhs;
    *(u8 mut*) rhs = tmp;
}

void Cstl_swap_16(AddrMut const lhs, AddrMut const rhs) {
    u16 const tmp = *(u16 const*) lhs;
    *(u16 mut*) lhs = *(u16 const*) rhs;
    *(u16 mut*) rhs = tmp;
}

void Cstl_swap_32(AddrMut const lhs, AddrMut const rhs) {
    u32 const tmp = *(u32 const*) lhs;
    *(u32 mut*) lhs = *(u32 const*) rhs;
    *(u32 mut*) rhs = tmp;
}

void Cstl_swap_64(AddrMut const lhs, AddrMut const rhs) {
    u64 const tmp = *(u64 const*) lhs;
    *(u64 mut*) lhs = *(u64 const*) rhs;
    *(u64 mut*) rhs = tmp;
}