#define USING_NAMESPACE_CSTL

#include <stdio.h>
#include <stdlib.h>

#include "slice.h"
#include "error.h"
#include "util.h"

#define elem_size_of(meta) Cstl_CollectionMeta_elem_size(meta)

#define new_meta(elem_size) Cstl_SliceMetaFlags_new(elem_size)



Cstl_SliceMetaFlags Cstl_SliceMetaFlags_new(usize const elem_size) {
    return elem_size;
}

AddrMut Cstl_Slice_get_unchecked(
    Cstl_Slice const* const self, usize const index
) {
    return (u8*) self->ptr + elem_size_of(self->meta) * index;
}

void* Cstl_Slice_get(Cstl_Slice const* const self, usize const index) {
    Cstl_assert(index < self->len);

    return Cstl_Slice_get_unchecked(self, index);
}

Cstl_Slice Cstl_Slice_from_raw(
    AddrMut const ptr,
    usize const len,
    usize const elem_size
) {
    return (Cstl_Slice) {
        .ptr = ptr,
        .len = len,
        .meta = new_meta(elem_size)
    };
}

void Cstl_Slice_sort_unstable(
    Cstl_Slice mut* const self,
    Cstl_Comparator const cmp
) {
    if (self->len <= 1) {
        return;
    } else if (2 == self->len) {
        if (Cstl_Slice_cmp_elems_unchecked(self, 0, 1, cmp) > 0) {
            Cstl_Slice_swap_unchecked(self, 0, 1);
        }

        return;
    } else if (self->len <= 64) {
        Cstl_Slice_binary_insert_sort(self, cmp);

        return;
    }

    usize const pivot_idx = Cstl_Slice_partition_at(self, 0, cmp);

    Cstl_Slice mut left
        = Cstl_Slice_slice_unchecked(self, 0, pivot_idx);

    Cstl_Slice mut right
        = Cstl_Slice_slice_unchecked(self, pivot_idx + 1, self->len);

    Cstl_Slice_sort_unstable(&mut left, cmp);
    Cstl_Slice_sort_unstable(&mut right, cmp);
}

void Cstl_Slice_binary_insert_sort(
    Cstl_Slice mut* const self,
    Cstl_Comparator const cmp
) {
    if (self->len <= 1) {
        return;
    }

    if (Cstl_Slice_cmp_elems_unchecked(self, 0, 1, cmp) > 0) {
        Cstl_Slice_swap_unchecked(self, 0, 1);
    }

    Cstl_Slice mut sorted = Cstl_Slice_slice(self, 0, 2);

    while (sorted.len < self->len) {
        usize mut insert_idx = Cstl_Slice_binary_search(
            &sorted,
            Cstl_Slice_get_unchecked(self, sorted.len),
            cmp
        );

        for (; insert_idx != sorted.len; ++insert_idx) {
            Cstl_Slice_swap_unchecked(self, insert_idx, sorted.len);
        }
        
        sorted.len += 1;
    }

    return;
}

usize Cstl_Slice_binary_search(
    Cstl_Slice mut* const self,
    Addr const value_ptr,
    Cstl_Comparator const cmp
) {
    usize mut l = 0, mut r = self->len;

    while (l < r) {
        usize const mid_idx = (l + r) / 2;

        Cstl_Ordering const order
            = cmp(Cstl_Slice_get_unchecked(self, mid_idx), value_ptr);

        switch (order) {
        case Cstl_Ordering_Less: {
            l = mid_idx + 1;
        } break;

        case Cstl_Ordering_Greater: {
            r = mid_idx;
        } break;

        case Cstl_Ordering_Equal: {
            l = r = mid_idx;
        } break;

        default:
            Cstl_deny_fmt(
                "invalid enum Cstl_Ordering value {u32}", (u32) order
            );
        }
    }

    return r;
}

usize Cstl_Slice_partition_at(
    Cstl_Slice mut* const self,
    usize mut pivot_idx,
    Cstl_Comparator const cmp
) {
    for (usize mut i = 0; i < self->len; ++i) {
        if (i == pivot_idx) {
            continue;
        }

        Cstl_Ordering const order
            = Cstl_Slice_cmp_elems_unchecked(self, i, pivot_idx, cmp);

        if (Cstl_Ordering_Greater == order && i < pivot_idx) {
            Cstl_Slice_swap_unchecked(self, i, pivot_idx);
            pivot_idx = i;
        } else if (Cstl_Ordering_Less == order && pivot_idx < i) {
            Cstl_Slice_swap_unchecked(self, i, pivot_idx + 1);
            Cstl_Slice_swap_unchecked(self, pivot_idx, pivot_idx + 1);
            pivot_idx += 1;
        } else if (Cstl_Ordering_Equal == order && pivot_idx < i) {
            Cstl_Slice_swap_unchecked(self, i, pivot_idx + 1);
            pivot_idx += 1;
        }
    }

    return pivot_idx;
}

void Cstl_Slice_swap(Cstl_Slice mut* const self, const usize lhs, const usize rhs) {
    Cstl_assert(lhs < self->len && rhs < self->len);

    if (lhs == rhs) {
        return;
    }

    Cstl_Slice_swap_unchecked(self, lhs, rhs);
}

void Cstl_Slice_swap_unchecked(
    Cstl_Slice mut* const self, usize const lhs, usize const rhs
) {
    Cstl_swap(
        (u8*) self->ptr + elem_size_of(self->meta) * lhs,
        (u8*) self->ptr + elem_size_of(self->meta) * rhs,
        elem_size_of(self->meta)
    );
}

Cstl_Slice Cstl_Slice_slice_unchecked(
    Cstl_Slice mut* const self, usize const start, usize mut end
) {
    if (end < start) {
        end = start;
    }

    return (Cstl_Slice) {
        .ptr = (u8*) self->ptr + start,
        .len = end - start,
        .meta = self->meta
    };
}

Cstl_Slice Cstl_Slice_slice(
    Cstl_Slice mut* const self, usize const start, usize const end
) {
    Cstl_assert(start < self->len && end <= self->len);

    return Cstl_Slice_slice_unchecked(self, start, end);
}

Cstl_Ordering Cstl_Slice_cmp(
    Cstl_Slice const* const lhs,
    Cstl_Slice const* const rhs,
    Cstl_Comparator const cmp
) {
    usize const elem_size = elem_size_of(lhs->meta);

    Cstl_assert(elem_size == elem_size_of(rhs->meta));

    if (0 == lhs->len && 0 == rhs->len) {
        return Cstl_Ordering_Equal;
    } else if (0 == lhs->len) {
        return Cstl_Ordering_Less;
    } else if (0 == rhs->len) {
        return Cstl_Ordering_Greater;
    }

    usize mut ne_idx = 0;

    while (Cstl_Ordering_Equal == cmp(
        (u8*)  lhs->ptr + elem_size * ne_idx,
        (u8*)  rhs->ptr + elem_size * ne_idx
    )) {
        ne_idx += 1;
    }

    if (ne_idx == lhs->len && ne_idx == rhs->len) {
        return Cstl_Ordering_Equal;
    } else if (ne_idx == lhs->len) {
        return Cstl_Ordering_Less;
    } else if (ne_idx == rhs->len) {
        return Cstl_Ordering_Greater;
    }

    return cmp(
        (u8*) lhs->ptr + elem_size * ne_idx,
        (u8*) rhs->ptr + elem_size * ne_idx
    );
}

Cstl_Ordering Cstl_Slice_cmp_elems_unchecked(
    Cstl_Slice const* const self,
    usize const lhs,
    usize const rhs,
    Cstl_Comparator const cmp
) {
    return cmp(
        Cstl_Slice_get_unchecked(self, lhs),
        Cstl_Slice_get_unchecked(self, rhs)
    );
}

Cstl_Ordering Cstl_Slice_cmp_elems(
    Cstl_Slice const* const self,
    usize const lhs,
    usize const rhs,
    Cstl_Comparator const cmp
) {
    Cstl_assert(lhs < self->len && rhs < self->len);

    return Cstl_Slice_cmp_elems_unchecked(self, lhs, rhs, cmp);
}

void Cstl_Slice_heapify(
    Cstl_Slice* const self,
    usize const index,
    Cstl_Ordering const target_order,
    Cstl_Comparator const cmp
) {
    usize mut extremum_idx = index;
    usize const l_idx = index * 2 + 1;
    usize const r_idx = index * 2 + 2;

    if (l_idx < self->len && Cstl_Ordering_reverse(target_order)
        == Cstl_Slice_cmp_elems_unchecked(self, extremum_idx, l_idx, cmp)
    ) {
        extremum_idx = l_idx;
    }

    if (r_idx < self->len && Cstl_Ordering_reverse(target_order)
        == Cstl_Slice_cmp_elems_unchecked(self, extremum_idx, r_idx, cmp)
    ) {
        extremum_idx = r_idx;
    }

    if (index != extremum_idx) {
        Cstl_Slice_swap_unchecked(self, index, extremum_idx);
        Cstl_Slice_heapify(self, extremum_idx, target_order, cmp);
    }
}

void Cstl_Slice_build_heap(
    Cstl_Slice mut* const self,
    Cstl_Ordering const target_order,
    Cstl_Comparator const cmp
) {
    if (self->len <= 1) {
        return;
    }

    for (usize mut i = self->len / 2 - 1; i + 1 != 0; --i) {
        Cstl_Slice_heapify(self, i, target_order, cmp);
    }
}

void Cstl_Slice_max_heapify(
    Cstl_Slice mut* const self,
    usize const index,
    Cstl_Comparator const cmp)
{
    Cstl_Slice_heapify(self, index, Cstl_Ordering_Greater, cmp);
}

void Cstl_Slice_build_maxheap(
    Cstl_Slice mut* const self,
    Cstl_Comparator const cmp
) {
    Cstl_Slice_build_heap(self, Cstl_Ordering_Greater, cmp);
}

void Cstl_Slice_min_heapify(
    Cstl_Slice mut* const self,
    usize const index,
    Cstl_Comparator const cmp
) {
    Cstl_Slice_heapify(self, index, Cstl_Ordering_Less, cmp);
}

void Cstl_Slice_build_minheap(
    Cstl_Slice mut* const self,
    Cstl_Comparator const cmp
) {
    Cstl_Slice_build_heap(self, Cstl_Ordering_Less, cmp);
}

void Cstl_Slice_heap_sort(
    Cstl_Slice mut* const self,
    Cstl_Comparator const cmp
) {
    if (0 == self->len) {
        return;
    }

    Cstl_Slice_build_maxheap(self, cmp);

    Cstl_Slice mut heap = Cstl_Slice_slice(self, 0, self->len - 1);

    for (; heap.len > 0; heap.len -= 1) {
        Cstl_Slice_swap_unchecked(self, 0, heap.len);
        Cstl_Slice_max_heapify(&heap, 0, cmp);
    }
}

void Cstl_Slice_reverse(Cstl_Slice mut self) {
    for (usize mut i = 0; i < self.len / 2; ++i) {
        Cstl_Slice_swap_unchecked(&mut self, i, self.len - i - 1);
    }
}



#define Cstl_impl_basic_type_slice(Type) \
    Type* Cstl_Slice_ ## Type ## _get_unchecked( \
        Cstl_Slice_ ## Type self, usize index \
    ) { \
        return self.ptr + index; \
    } \
    \
    Type* Cstl_Slice_ ## Type ## _get(Cstl_Slice_ ## Type self, usize index) { \
        Cstl_assert(index < self.len); \
        return Cstl_Slice_ ## Type ## _get_unchecked(self, index); \
    }

Cstl_impl_basic_type_slice(u8);
Cstl_impl_basic_type_slice(i8);
Cstl_impl_basic_type_slice(u16);
Cstl_impl_basic_type_slice(i16);
Cstl_impl_basic_type_slice(u32);
Cstl_impl_basic_type_slice(i32);
Cstl_impl_basic_type_slice(u64);
Cstl_impl_basic_type_slice(i64);
Cstl_impl_basic_type_slice(usize);
Cstl_impl_basic_type_slice(isize);
Cstl_impl_basic_type_slice(f32);
Cstl_impl_basic_type_slice(f64);



