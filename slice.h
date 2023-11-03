#ifndef _CSTL_SLICE_H_
#define _CSTL_SLICE_H_

#include "types.h"
#include "util.h"
#include "compare.h"



#define Cstl_Slice_from_elems(Type, elems...) \
    Cstl_Slice_from_raw( \
        (Type[]) { elems }, \
        Cstl_array_len(((Type[]) { elems })), \
        sizeof(*((Type[]) { elems })) \
    )

#define Cstl_Slice_get_value(self, Type, index) \
    (*(Type*) Cstl_Slice_get((self), (index)))



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

    #define Slice_get_value(self, Type, index) \
        Cstl_Slice_get_value(self, Type, index)
    
    typedef Cstl_Slice Slice;

    AddrMut Slice_get(Slice const* self, usize index);

    /// @brief Computes lexicographical ordering of two slices.
    /// 
    /// @param cmp a comparator of slice elements.
    /// 
    /// @return Lexicographical ordering.
    Ordering Slice_cmp(
        Slice const* lhs,
        Slice const* rhs,
        Cstl_Comparator cmp
    );

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_SLICE_H_