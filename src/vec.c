#define USING_NAMESPACE_CSTL

#include "../include/cstl/vec.h"
#include "../include/cstl/error.h"
#include "../include/cstl/memory.h"



Cstl_CollectionMeta Cstl_Vec_new_meta(usize const elem_size) {
    return elem_size;
}

Cstl_Vec Cstl_Vec_new(usize const elem_size) {
    return (Cstl_Vec) {
        .cap = 0,
        .len = 0,
        .ptr = null_mut,
        .meta = Vec_new_meta(elem_size)
    };
}

Cstl_Vec Cstl_Vec_with_capacity(usize const cap, usize const elem_size) {
    return (Cstl_Vec) {
        .cap = cap,
        .len = 0,
        .ptr = Cstl_mem_alloc_unaligned(elem_size * cap),
        .meta = Vec_new_meta(elem_size)
    };
}

Cstl_Vec Cstl_Vec_from_slice(Cstl_Slice const src) {
    Cstl_Vec mut result
        = Cstl_Vec_with_capacity(src.len, elem_size_of(src.meta));

    result.len = src.len;

    Cstl_mem_copy(result.ptr, src.ptr, elem_size_of(src.meta) * src.len);

    return result;
}

Cstl_Vec Cstl_Vec_clone(Cstl_Vec const* const self) {
    Cstl_Vec const result
        = Cstl_Vec_with_capacity(self->len, elem_size_of(self->meta));

    Cstl_mem_copy(result.ptr, self->ptr, elem_size_of(result.meta) * self->len);

    return result;
}

void Cstl_Vec_clone_from(Cstl_Vec mut* const self, Cstl_Vec const* const src) {
    Cstl_assert(elem_size_of(self->meta) == elem_size_of(src->meta));

    if (self->cap < src->cap) {
        Cstl_Vec_free(self);
        *self = Cstl_Vec_clone(src);
    }
    
    self->len = src->len;

    Cstl_mem_copy(self->ptr, src->ptr, elem_size_of(self->meta) * src->len);
}

void Cstl_Vec_free(Cstl_Vec const* const self) {
    Cstl_mem_free_unaligned(self->ptr);
    *(Cstl_Vec mut*) self = Cstl_Vec_new(elem_size_of(self->meta));
}

void Cstl__internal_Vec_alloc(Cstl_Vec mut* const self, usize const new_cap) {
    if (null_mut == self->ptr) {
        *self = Cstl_Vec_with_capacity(new_cap, elem_size_of(self->meta));
        return;
    }

    if (new_cap <= self->cap) {
        return;
    }

    self->ptr = Cstl_mem_realloc_unaligned(self->ptr, elem_size_of(self->meta) * new_cap);
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

void Cstl_Vec_push(Cstl_Vec mut* const self, Addr const elem_ptr) {
    AddrMut const in_place_elem_ptr = Cstl_Vec_push_in_place(self);
    Cstl_mem_copy(in_place_elem_ptr, elem_ptr, elem_size_of(self->meta));
}

AddrMut Cstl_Vec_pop(Cstl_Vec mut* const self) {
    Cstl_assert(0 < self->len);

    return (u8 mut*)  self->ptr + elem_size_of(self->meta) * (--self->len);
}

void Cstl_Vec_reserve_exact(
    Cstl_Vec mut* const self, usize const additional_cap
) {
    if (self->len + additional_cap <= self->cap) {
        return;
    }

    Cstl__internal_Vec_alloc(self, additional_cap + self->len - self->cap);
}

void Cstl_Vec_retype(Cstl_Vec mut* const self, usize const new_elem_size) {
    assert_msg(0 == self->len, "only empty `Vec`s can be retyped");

    usize const old_n_bytes = self->cap * elem_size_of(self->meta);
    usize const new_cap = old_n_bytes / new_elem_size;
    usize const n_lost_bytes = old_n_bytes - new_cap * new_elem_size;

    if (0 < n_lost_bytes) {
        self->ptr = Cstl_mem_realloc_unaligned(
            self->ptr, old_n_bytes + new_elem_size - n_lost_bytes
        );
    }

    self->cap = new_cap;
    self->meta = Cstl_Vec_new_meta(new_elem_size);
}

void Cstl_Vec_reserve(Cstl_Vec mut* const self, usize const additional_cap) {
    if (self->len + additional_cap <= self->len) {
        return;
    }

    usize const cap_sum = self->cap + additional_cap;

    usize mut next_cap
        = Cstl__internal_Vec_next_capacity(self->cap, elem_size_of(self->meta));

    next_cap = usize_max(next_cap, cap_sum);

    Cstl_Vec_reserve_exact(self, next_cap);
}

void Cstl_Vec_clear(Cstl_Vec mut* const self) {
    self->len = 0;
}

void Cstl_Vec_shrink_to(Cstl_Vec mut* const self, usize const min_cap) {
    if (self->cap <= min_cap) {
        return;
    }

    self->ptr = Cstl_mem_realloc_unaligned(self->ptr, min_cap);
    self->cap = min_cap;
}

void Cstl_Vec_shrink_to_fit(Cstl_Vec mut* const self) {
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

    Cstl_mem_copy(
        (u8 mut*) self->ptr + elem_size_of(self->meta) * index,
        value_ptr,
        elem_size_of(self->meta)
    );
}

void Cstl_Vec_fill(Cstl_Vec const* const self, Addr const value_ptr) {
    bool const is_out_of_bounds
        = value_ptr < self->ptr
        && (u8 mut*) self->ptr + elem_size_of(self->meta) * self->len
            <= (u8 mut*) value_ptr;

    if (is_out_of_bounds) {
        for (usize mut i = 0; i < self->len; ++i) {
            Cstl_mem_copy(
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
                Cstl_mem_copy(cur_ptr, value_ptr, elem_size_of(self->meta));
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

    Cstl_Slice_swap(*(Cstl_Slice const*) self, 0, self->len - 1);

    Cstl_Slice mut smaller_slice =
        Cstl_Slice_slice(*(Cstl_Slice const*) self, 0, self->len - 1);

    switch (extremum_type) {
    case Cstl_Ordering_Less:
        Cstl_Slice_min_heapify(smaller_slice, 0, cmp);
        break;
        
    case Cstl_Ordering_Greater:
        Cstl_Slice_max_heapify(smaller_slice, 0, cmp);
        break;

    case Cstl_Ordering_Equal:
        // fallthrough
    default:
        Cstl_deny_fmt("invalid extremum_type value {u32}", (u32) extremum_type);
    }

    return Cstl_Vec_pop(self);
}

Cstl_Slice Cstl_Vec_slice(
    Cstl_Vec mut* const self, usize const start, usize const end
) {
    Cstl_assert(start < self->len && end < self->len);
    return Cstl_Vec_slice_unchecked(self, start, end);
}

Cstl_Slice Cstl_Vec_slice_unchecked(
    Cstl_Vec mut* const self, usize const start, usize const end
) {
    return (Cstl_Slice) {
        .ptr = (u8 mut*) self->ptr + start,
        .len = (start <= end) * (end - start),
        .meta = self->meta
    };
}



#define Cstl_impl_typed_Vec(Type) \
    Cstl_Vec_##Type Cstl_Vec_##Type##_new(void) { \
        return Cstl_Vec_##Type##_DEFAULT; \
    } \
    Cstl_Vec_##Type Cstl_Vec_##Type##_with_capacity(usize const cap) { \
        return (Cstl_Vec_##Type) { \
            .ptr = Cstl_mem_alloc_unaligned(sizeof(Type) * cap), \
            .cap = cap, \
            .len = 0 \
        }; \
    } \
    Cstl_Vec_##Type Cstl_Vec_##Type##_repeat(usize const count, Type const value) { \
        Cstl_Vec_##Type mut result = Cstl_Vec_##Type##_with_capacity(count); \
        result.len = count; \
        Cstl_Vec_##Type##_fill(&mut result, value); \
        return result; \
    } \
    Cstl_Vec_##Type Cstl_Vec_##Type##_from_typed_slice(Cstl_Slice_##Type const src) { \
        Cstl_Vec_##Type mut result = Cstl_Vec_##Type##_with_capacity(src.len); \
        Cstl_mem_copy((u8 mut*) result.ptr, (u8 const*) src.ptr, sizeof(Type) * src.len); \
        result.len = src.len; \
        return result; \
    } \
    Cstl_Vec_##Type Cstl_Vec_##Type##_clone(Cstl_Vec_##Type const* const self) { \
        return Cstl_Vec_##Type##_from_typed_slice(Cstl_Vec_##Type##_as_typed_slice(self)); \
    } \
    void Cstl_Vec_##Type##_clone_from(Cstl_Vec_##Type mut* const self, Cstl_Vec_##Type const* const src) { \
        if (self->cap < src->cap) { \
            Cstl_Vec_##Type##_free(self); \
            *self = Cstl_Vec_##Type##_clone(src); \
        } \
        self->len = src->len; \
        Cstl_mem_copy((u8 mut*) self->ptr, (u8 const*) src->ptr, sizeof(Type) * src->len); \
    } \
    void Cstl_Vec_##Type##_free(Cstl_Vec_##Type const* self) { \
        Cstl_mem_free_unaligned(self->ptr); \
        *(Cstl_Vec_##Type mut*) self = Cstl_Vec_##Type##_DEFAULT; \
    } \
    void Cstl__internal_Vec_##Type##_alloc(Cstl_Vec_##Type mut* const self, usize const new_cap) { \
        if (null_mut == self->ptr) { \
            *self = Cstl_Vec_##Type##_with_capacity(new_cap); \
            return; \
        } \
        if (new_cap <= self->cap) { \
            return; \
        } \
        self->ptr = Cstl_mem_realloc_unaligned(self->ptr, sizeof(Type) * new_cap); \
        self->cap = new_cap; \
    } \
    usize Cstl__internal_Vec_##Type##_next_capacity(usize const cur_cap) { \
        if (cur_cap <= 1) { \
            if (sizeof(Type) <= 16) { \
                return 8; \
            } else if (sizeof(Type) <= 256) { \
                return 4; \
            } else { \
                return 2; \
            } \
        } \
        return 3 * cur_cap / 2; \
    } \
    void Cstl_Vec_##Type##_push(Cstl_Vec_##Type mut* const self, Type const value) { \
        *Cstl_Vec_##Type##_push_in_place(self) = value; \
    } \
    Type Cstl_Vec_##Type##_pop(Cstl_Vec_##Type mut* const self) { \
        Cstl_assert(0 < self->len); \
        return self->ptr[--self->len]; \
    } \
    void Cstl_Vec_##Type##_reserve_exact(Cstl_Vec_##Type mut* const self, usize const additional_cap) { \
        Cstl__internal_Vec_##Type##_alloc(self, self->cap + additional_cap); \
    } \
    void Cstl_Vec_##Type##_reserve(Cstl_Vec_##Type mut* self, usize additional_cap) { \
        usize const cap_sum = self->cap + additional_cap; \
        usize mut next_cap = Cstl__internal_Vec_##Type##_next_capacity(self->cap); \
        next_cap = next_cap < cap_sum ? cap_sum : next_cap; \
        Cstl_Vec_##Type##_reserve_exact(self, next_cap); \
    } \
    void Cstl_Vec_##Type##_clear(Cstl_Vec_##Type mut* const self) { \
        self->len = 0; \
    } \
    void Cstl_Vec_##Type##_shrink_to(Cstl_Vec_##Type mut* const self, usize const min_cap) { \
        if (self->cap <= min_cap) { return; } \
        self->ptr = Cstl_mem_realloc_unaligned(self->ptr, min_cap); \
        self->cap = min_cap; \
    } \
    void Cstl_Vec_##Type##_shrink_to_fit(Cstl_Vec_##Type mut* const self) { \
        Cstl_Vec_##Type##_shrink_to(self, self->len); \
    } \
    Type mut* Cstl_Vec_##Type##_get(Cstl_Vec_##Type const* const self, usize const index) { \
        Cstl_assert(index < self->len); \
        return &mut self->ptr[index]; \
    } \
    void Cstl_Vec_##Type##_set(Cstl_Vec_##Type const* const self, usize const index, Type const value) { \
        Cstl_assert(index < self->len); \
        self->ptr[index] = value; \
    } \
    void Cstl_Vec_##Type##_fill(Cstl_Vec_##Type const* const self, Type const value) { \
        for (usize mut i = 0; i < self->len; ++i) { \
            self->ptr[i] = value; \
        } \
    } \
    Cstl_Slice Cstl_Vec_##Type##_as_slice(Cstl_Vec_##Type const* const self) { \
        return (Cstl_Slice) { \
            .ptr = self->ptr, \
            .len = self->len, \
            .meta = sizeof(Type) \
        }; \
    } \
    Cstl_Slice_##Type Cstl_Vec_##Type##_as_typed_slice(Cstl_Vec_##Type const* const self) { \
        return (Cstl_Slice_##Type) { \
            .ptr = self->ptr, \
            .len = self->len \
        }; \
    } \
    Type mut* Cstl_Vec_##Type##_push_in_place(Cstl_Vec_##Type mut* const self) { \
        if (self->len == self->cap) { \
            usize const new_cap = Cstl__internal_Vec_##Type##_next_capacity(self->cap); \
            Cstl__internal_Vec_##Type##_alloc(self, new_cap); \
        } \
        return &mut self->ptr[self->len++]; \
    } \
    Type Cstl_Vec_##Type##_extract(Cstl_Vec_##Type mut* const self, Cstl_Ordering const extremum_type) { \
        Cstl_assert(0 < self->len); \
        Cstl_Slice_swap(Cstl_Vec_##Type##_as_slice(self), 0, self->len - 1); \
        Cstl_Slice mut smaller_slice = Cstl_Vec_##Type##_slice_unchecked(self, 0, self->len - 1); \
        switch (extremum_type) { \
        case Cstl_Ordering_Less:    Cstl_Slice_min_heapify(smaller_slice, 0, Type##_cmp); break; \
        case Cstl_Ordering_Greater: Cstl_Slice_max_heapify(smaller_slice, 0, Type##_cmp); break; \
        case Cstl_Ordering_Equal:   default: \
            Cstl_deny_fmt("invalid extremum_type value {u32}", (u32) extremum_type); \
        } \
        return Cstl_Vec_##Type##_pop(self); \
    } \
    Type Cstl_Vec_##Type##_extract_min(Cstl_Vec_##Type mut* const self) { \
        return Cstl_Vec_##Type##_extract(self, Cstl_Ordering_Less); \
    } \
    Type Cstl_Vec_##Type##_extract_max(Cstl_Vec_##Type mut* const self) { \
        return Cstl_Vec_##Type##_extract(self, Cstl_Ordering_Greater); \
    } \
    Cstl_Slice Cstl_Vec_##Type##_slice(Cstl_Vec_##Type const* const self, usize const start, usize const end) { \
        Cstl_assert(start < self->len && end <= self->len); \
        return Cstl_Vec_##Type##_slice_unchecked(self, start, end); \
    } \
    Cstl_Slice_##Type Cstl_Vec_##Type##_slice_typed(Cstl_Vec_##Type const* const self, usize const start, usize const end) { \
        Cstl_assert(start < self->len && end <= self->len); \
        return Cstl_Vec_##Type##_slice_typed_unchecked(self, start, end); \
    } \
    Cstl_Slice Cstl_Vec_##Type##_slice_unchecked(Cstl_Vec_##Type const* const self, usize const start, usize const end) { \
        return (Cstl_Slice) { \
            .ptr = self->ptr + start, \
            .len = start <= end ? end - start : 0, \
            .meta = sizeof(Type) \
        }; \
    } \
    Cstl_Slice_##Type Cstl_Vec_##Type##_slice_typed_unchecked(Cstl_Vec_##Type const* const self, usize const start, usize const end) { \
        return (Cstl_Slice_##Type) { \
            .ptr = self->ptr + start, \
            .len = start <= end ? end - start : 0 \
        }; \
    } \
    Cstl_Vec Cstl_Vec_##Type##_to_untyped(Cstl_Vec_##Type const* const self) { \
        return (Cstl_Vec) { \
            .ptr = self->ptr, \
            .len = self->len, \
            .cap = self->cap, \
            .meta = sizeof(Type) \
        }; \
    }

Cstl_impl_typed_Vec(u8)
Cstl_impl_typed_Vec(i8)
Cstl_impl_typed_Vec(u16)
Cstl_impl_typed_Vec(i16)
Cstl_impl_typed_Vec(u32)
Cstl_impl_typed_Vec(i32)
Cstl_impl_typed_Vec(u64)
Cstl_impl_typed_Vec(i64)
Cstl_impl_typed_Vec(usize)
Cstl_impl_typed_Vec(isize)
Cstl_impl_typed_Vec(char)
Cstl_impl_typed_Vec(bool)
Cstl_impl_typed_Vec(f32)
Cstl_impl_typed_Vec(f64)