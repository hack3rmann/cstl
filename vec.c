#define USING_NAMESPACE_CSTL

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vec.h"
#include "error.h"



Cstl_CollectionMeta Cstl_Vec_new_meta(usize const elem_size) {
    return elem_size;
}

Cstl_Vec Cstl_Vec_new(usize const elem_size) {
    return (Cstl_Vec) {
        .cap = 0,
        .len = 0,
        .ptr = NULL,
        .meta = Vec_new_meta(elem_size)
    };
}

Cstl_Vec Cstl_Vec_with_capacity(usize const cap, usize const elem_size) {
    return (Cstl_Vec) {
        .cap = cap,
        .len = 0,
        .ptr = malloc(elem_size * cap),
        .meta = Vec_new_meta(elem_size)
    };
}

Cstl_Vec Cstl_Vec_from_slice(Cstl_Slice const src) {
    Cstl_Vec mut result
        = Cstl_Vec_with_capacity(src.len, elem_size_of(src.meta));

    result.len = src.len;

    memcpy(result.ptr, src.ptr, elem_size_of(src.meta) * src.len);

    return result;
}

Cstl_Vec Cstl_Vec_clone(Cstl_Vec const* const self) {
    Cstl_Vec const result
        = Cstl_Vec_with_capacity(self->len, elem_size_of(self->meta));

    memcpy(result.ptr, self->ptr, elem_size_of(result.meta) * self->len);

    return result;
}

void Cstl_Vec_clone_from(Cstl_Vec* const self, Cstl_Vec const* const src) {
    Cstl_assert(elem_size_of(self->meta) == elem_size_of(src->meta));

    if (self->cap < src->cap) {
        Cstl_Vec_free(self);
        *self = Cstl_Vec_clone(src);
    }
    
    self->len = src->len;

    memcpy(self->ptr, src->ptr, elem_size_of(self->meta) * src->len);
}

void Cstl_Vec_free(Cstl_Vec const* const self) {
    if (NULL != self->ptr) {
        free(self->ptr);
    }

    *(Cstl_Vec mut*) self = Cstl_Vec_new(elem_size_of(self->meta));
}

void Cstl__internal_Vec_alloc(Cstl_Vec* const self, usize const new_cap) {
    if (NULL == self->ptr) {
        *self = Cstl_Vec_with_capacity(new_cap, elem_size_of(self->meta));
        return;
    }

    if (new_cap <= self->cap) {
        return;
    }

    self->ptr = realloc(self->ptr, elem_size_of(self->meta) * new_cap);
    self->cap = new_cap;
}

usize Cstl__internal_Vec_next_capacity(
    usize const cur_cap,
    usize const elem_size
) {
    if (cur_cap <= 1) {
        if (elem_size <= 16) {
            return 8;
        } else if (elem_size <= 256) {
            return 4;
        } else {
            return 2;
        }
    }

    return 3 * cur_cap / 2;
}

void Cstl_Vec_push(Cstl_Vec* const self, Addr const elem_ptr) {
    void mut* const in_place_elem_ptr = Cstl_Vec_push_in_place(self);
    memcpy(in_place_elem_ptr, elem_ptr, elem_size_of(self->meta));
}

AddrMut Cstl_Vec_pop(Cstl_Vec* const self) {
    Cstl_assert(0 < self->len);

    return (u8 mut*)  self->ptr + elem_size_of(self->meta) * (--self->len);
}

void Cstl_Vec_reserve_exact(
    Cstl_Vec* const self, usize const additional_cap
) {
    Cstl__internal_Vec_alloc(self, self->cap + additional_cap);
}

void Cstl_Vec_reserve(Cstl_Vec* const self, usize const additional_cap) {
    usize const cap_sum = self->cap + additional_cap;

    usize next_cap
        = Cstl__internal_Vec_next_capacity(self->cap, elem_size_of(self->meta));

    next_cap = next_cap < cap_sum ? cap_sum : next_cap;

    Cstl_Vec_reserve_exact(self, next_cap);
}

void Cstl_Vec_clear(Cstl_Vec* const self) {
    self->len = 0;
}

void Cstl_Vec_shrink_to(Cstl_Vec* const self, usize const min_cap) {
    if (self->cap <= min_cap) {
        return;
    }

    self->ptr = realloc(self->ptr, min_cap);
    self->cap = min_cap;
}

void Cstl_Vec_shrink_to_fit(Cstl_Vec* const self) {
    Cstl_Vec_shrink_to(self, self->len);
}

AddrMut Cstl_Vec_get(Cstl_Vec const* const self, usize const index) {
    Cstl_assert(index < self->len);

    return (u8 mut*) self->ptr + elem_size_of(self->meta) * index;
}

void Cstl_Vec_set(
    Cstl_Vec const* const self, usize const index, Addr const value_ptr
) {
    Cstl_assert(index < self->len);

    memcpy(
        (u8 mut*) self->ptr + elem_size_of(self->meta) * index,
        value_ptr,
        elem_size_of(self->meta)
    );
}

void Cstl_Vec_fill(Cstl_Vec const* const self, Addr const value_ptr) {
    Bool const is_out_of_bounds
        = value_ptr < self->ptr
        && (u8 mut*) self->ptr + elem_size_of(self->meta) * self->len
            <= (u8 mut*) value_ptr;

    if (is_out_of_bounds) {
        for (usize mut i = 0; i < self->len; ++i) {
            memcpy(
                (u8 mut*) self->ptr + elem_size_of(self->meta) * i,
                value_ptr,
                elem_size_of(self->meta)
            );
        }
    } else {
        for (usize mut i = 0; i < self->len; ++i) {
            AddrMut const cur_ptr
                = (u8 mut*) self->ptr + elem_size_of(self->meta) * i;

            if (cur_ptr != value_ptr) {
                memcpy(cur_ptr, value_ptr, elem_size_of(self->meta));
            }
        }
    }
}

Cstl_Slice Cstl_Vec_as_slice(Cstl_Vec const* const self) {
    return (Cstl_Slice) {
        .meta = self->meta,
        .len = self->len,
        .ptr = self->ptr
    };
}

AddrMut Cstl_Vec_push_in_place(Cstl_Vec mut* const self) {
    if (self->len == self->cap) {
        usize const new_cap = Cstl__internal_Vec_next_capacity(
            self->cap,
            elem_size_of(self->meta)
        );

        Cstl__internal_Vec_alloc(self, new_cap);
    }

    return (u8 mut*) self->ptr + elem_size_of(self->meta) * (self->len++);
}

AddrMut Cstl_Vec_extract(
    Cstl_Vec mut* const self,
    Cstl_Ordering const extremum_type,
    Cstl_Comparator const cmp
) {
    Cstl_assert(0 < self->len);

    Cstl_Slice_swap((Cstl_Slice mut*) self, 0, self->len - 1);

    Cstl_Slice mut smaller_slice =
        Cstl_Slice_slice((Cstl_Slice mut*) self, 0, self->len - 1);

    switch (extremum_type) {
    case Cstl_Ordering_Less: {
        Cstl_Slice_min_heapify(&mut smaller_slice, 0, cmp);
    } break;

    case Cstl_Ordering_Greater: {
        Cstl_Slice_max_heapify(&mut smaller_slice, 0, cmp);
    } break;

    case Cstl_Ordering_Equal:
        // fallthrough
    default:
        Cstl_deny_fmt("invalid extremum_type value %u", (u32) extremum_type);
    }

    Cstl_assert(False);

    return Cstl_Vec_pop(self);
}

Cstl_Slice Cstl_Vec_slice(
    Cstl_Vec mut* const self, usize const start, usize const end
) {
    Cstl_assert(start < self->len && end < self->len);
    return Cstl_Vec_slice_unchecked(self, start, end);
}

Cstl_Slice Cstl_Vec_slice_unchecked(
    Cstl_Vec mut* const self, usize const start, usize const end)
{
    return (Cstl_Slice) {
        .ptr = (u8 mut*) self->ptr + start,
        .len = (start <= end) * (end - start),
        .meta = self->meta
    };
}
