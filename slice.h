#ifndef _CSTL_SLICE_H_
#define _CSTL_SLICE_H_

#include "types.h"
#include "util.h"
#include "compare.h"



#define Cstl_Slice_from_elems(Type, elems...) \
    (Cstl_Slice) { \
        .ptr = (Type[]) { elems }, \
        .len = Cstl_array_len(((Type[]) { elems })), \
        .meta = sizeof(*((Type[]) { elems })) \
    }

#define Cstl_Slice_get_value(self, Type, index) \
    (*(Type mut*) Cstl_Slice_get((self), (index)))



typedef enum {
    Cstl_SliceMetaFlags_Default = (u16) 0
} Cstl_SliceMetaFlags;

Cstl_SliceMetaFlags Cstl_SliceMetaFlags_new(usize elem_size);



typedef struct {
    AddrMut ptr;
    usize len;
    Cstl_CollectionMeta meta;
} Cstl_Slice;

AddrMut Cstl_Slice_get_unchecked(Cstl_Slice const* self, usize index);

AddrMut Cstl_Slice_get(Cstl_Slice const* self, usize index);

Cstl_Slice Cstl_Slice_from_raw(AddrMut ptr, usize len, usize elem_size);

void Cstl_Slice_sort_unstable(
    Cstl_Slice mut* self, Cstl_Comparator cmp
);

void Cstl_Slice_binary_insert_sort(
    Cstl_Slice mut* self, Cstl_Comparator cmp
);

usize Cstl_Slice_binary_search(
    Cstl_Slice mut* self, Addr value_ptr, Cstl_Comparator cmp
);

usize Cstl_Slice_partition_at(
    Cstl_Slice mut* self, usize pivot, Cstl_Comparator cmp
);

void Cstl_Slice_swap(Cstl_Slice mut* self, usize lhs, usize rhs);

void Cstl_Slice_swap_unchecked(Cstl_Slice mut* self, usize lhs, usize rhs);

Cstl_Slice Cstl_Slice_slice_unchecked(Cstl_Slice mut* self, usize start, usize end);

Cstl_Slice Cstl_Slice_slice(Cstl_Slice mut* self, usize start, usize end);


/// @brief Computes lexicographical ordering of two slices.
/// 
/// @param cmp a comparator of slice elements.
/// 
/// @return Lexicographical ordering.
Cstl_Ordering Cstl_Slice_cmp(
    Cstl_Slice const* lhs, Cstl_Slice const* rhs, Cstl_Comparator cmp
);


Cstl_Ordering Cstl_Slice_cmp_elems_unchecked(
    Cstl_Slice const* self, usize lhs, usize rhs, Cstl_Comparator cmp
);

Cstl_Ordering Cstl_Slice_cmp_elems(
    Cstl_Slice const* self, usize lhs, usize rhs, Cstl_Comparator cmp
);

void Cstl_Slice_heapify(
    Cstl_Slice mut* self,
    usize index,
    Cstl_Ordering terget_order,
    Cstl_Comparator cmp
);

void Cstl_Slice_build_heap(
    Cstl_Slice mut* self, Cstl_Ordering target_order, Cstl_Comparator cmp
);

void Cstl_Slice_max_heapify(
    Cstl_Slice mut* self, usize index, Cstl_Comparator cmp
);

void Cstl_Slice_build_maxheap(
    Cstl_Slice mut* self, Cstl_Comparator cmp
);

void Cstl_Slice_min_heapify(
    Cstl_Slice mut* self, usize index, Cstl_Comparator cmp
);

void Cstl_Slice_build_minheap(
    Cstl_Slice mut* self, Cstl_Comparator cmp
);

void Cstl_Slice_heap_sort(
    Cstl_Slice mut* self, Cstl_Comparator cmp
);

void Cstl_Slice_reverse(Cstl_Slice self);



#define Cstl_declare_basic_type_slice(Type) \
    typedef struct { \
        Type* ptr; \
        usize len; \
    } Cstl_Slice_ ## Type; \
    \
    Type* Cstl_Slice_ ## Type ## _get_unchecked( \
        Cstl_Slice_ ## Type self, usize index \
    ); \
    \
    Type* Cstl_Slice_ ## Type ## _get(Cstl_Slice_ ## Type self, usize index);
    
Cstl_declare_basic_type_slice(u8);
Cstl_declare_basic_type_slice(i8);
Cstl_declare_basic_type_slice(u16);
Cstl_declare_basic_type_slice(i16);
Cstl_declare_basic_type_slice(u32);
Cstl_declare_basic_type_slice(i32);
Cstl_declare_basic_type_slice(u64);
Cstl_declare_basic_type_slice(i64);
Cstl_declare_basic_type_slice(usize);
Cstl_declare_basic_type_slice(isize);
Cstl_declare_basic_type_slice(f32);
Cstl_declare_basic_type_slice(f64);


#ifdef USING_NAMESPACE_CSTL

    #define Slice_from_elems(Type, elems...) \
        Cstl_Slice_from_elems(Type, elems)

    #define Slice(Type, elems...) \
        Slice_from_elems(Type, elems)

    #define Slice_get_value(self, Type, index) \
        Cstl_Slice_get_value(self, Type, index)

    typedef Cstl_SliceMetaFlags SliceMetaFlags;

    #define SliceMetaFlags_Default \
        Cstl_SliceMetaFlags_Default
    
    typedef Cstl_Slice Slice;

    #define Slice_get_unchecked(self, index) \
        Cstl_Slice_get_unchecked(self, index)

    #define Slice_get(self, index) \
        Cstl_Slice_get(self, index) 

    #define Slice_from_raw(ptr, len, elem_size) \
        Cstl_Slice_from_raw(ptr, len, elems_size)

    #define Slice_sort_unstable(self, cmp) \
        Cstl_Slice_sort_unstable(self, cmp)
    
    #define Slice_binary_insert_sort(self, cmp) \
        Cstl_Slice_binary_insert_sort(self, cmp)

    #define Slice_binary_search(self, value_ptr, cmp) \
        Cstl_Slice_binary_search(self, value_ptr, cmp)
    
    #define Slice_partition_at(self, pivot_idx, cmp) \
        Cstl_Slice_partition_at(self, picot_idx, cmp)

    #define Slice_swap(self, lhs, rhs) \
        Cstl_Slice_swap(self, lhs, rhs)
    
    #define Slice_swap_unchecked(self, lhs, rhs) \
        Cstl_Slice_swap_unchecked(self, lhs, rhs) 

    #define Slice_slice_unchecked(self, start, end) \
        Cstl_Slice_slice_unchecked(self, start, end)

    #define Slice_slice(self, start, end) \
        Cstl_Slice_slice(self, start, end)

    #define Slice_cmp(lhs, rhs, cmp) \
        Cstl_Slice_cmp(lhs, rhs, cmp)

    #define Slice_cmp_elems_unchecked(self, lhs, rhs, cmp) \
        Cstl_Slice_cmp_elems_unchecked(self, lhs, rhs, cmp)

    #define Slice_heapify(self, index, target_order, cmp) \
        Cstl_Slice_heapify(self, index, target_order, cmp)

    #define Slice_build_heap(self, target_order, cmp) \
        Cstl_Slice_build_heap(self, target_order, cmp)

    #define Slice_max_heapify(self, index, cmp) \
        Cstl_Slice_max_heapify(self, index, cmp)

    #define Slice_min_heapify(self, index, cmp) \
        Cstl_Slice_min_heapify(self, index, cmp)

    #define Slice_build_minheap(self, cmp) \
        Cstl_Slice_build_minheap(self, cmp)

    #define Slice_build_maxheap(self, cmp) \
        Cstl_Slice_build_maxheap(self, cmp)

    #define Slice_heap_sort(self, cmp) \
        Cstl_Slice_heap_sort(self, cmp)

    #define Slice_reverse(self) \
        Cstl_Slice_reverse(self)

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_SLICE_H_