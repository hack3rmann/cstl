#ifndef _CSTL_VEC_H_
#define _CSTL_VEC_H_

#include "slice.h"
#include "util.h"


#define Cstl_Vec_from_elems(Type, elems...) \
    Cstl_Vec_from_slice(Cstl_Slice_from_elems(Type, elems))

#define Cstl_Vec_push_value(self, Type, value) \
    Cstl_Vec_push((self), (Type[]) { (value) })



/// @brief Copies a value of type `Type` at index `index`.
/// 
/// @param Type a type of value.
/// @param index the index of value.
/// 
/// @return The value at index `index`.
#define Cstl_Vec_get_value(self, Type, index) \
    (*(Type mut*) Cstl_Vec_get((self), (index)))



/// @brief Dynamic array implementation in C.
typedef struct Cstl_Vec {
    AddrMut ptr;
    usize len;
    Cstl_CollectionMeta meta;
    usize cap;
} Cstl_Vec;


/// @brief Constructs new meta data for [`Vec`];
/// 
/// @param elem_size the size of an element of the vector, in bytes.
Cstl_CollectionMeta Cstl_Vec_new_meta(usize elem_size);


/// @brief Constructs an empty vector.
/// 
/// @param elem_size the size of an element of the vector.
/// 
/// @return New empty vector.
Cstl_Vec Cstl_Vec_new(usize elem_size);


/// @brief Constructs a vector with capacity `cap`.
/// 
/// @param cap start capacity of a vector.
/// @param elem_size the size of an element of the vector
/// 
/// @return New empty vector.
Cstl_Vec Cstl_Vec_with_capacity(usize cap, usize elem_size);


/// @brief Constructs a vector from slice.
/// 
/// @param src a source slice to copy.
/// 
/// @return New vector with `src` elements.
Cstl_Vec Cstl_Vec_from_slice(Cstl_Slice src);


/// @brief Clones the vector.
/// 
/// @return A vector clone.
Cstl_Vec Cstl_Vec_clone(Cstl_Vec const* self);


/// @brief Clones the vector to existing one.
/// 
/// @param src the vector to be clonned.
void Cstl_Vec_clone_from(Cstl_Vec mut* self, Cstl_Vec const* src);


/// @brief Frees a memory of the vector.
void Cstl_Vec_free(Cstl_Vec const* self);


void Cstl__internal_Vec_alloc(Cstl_Vec mut* self, usize new_cap);


usize Cstl__internal_Vec_next_capacity(usize cur_cap, usize elem_size);


/// @brief Pushes the value to the end of the vector.
/// 
/// @param value_ptr a pointer to the value.
void Cstl_Vec_push(Cstl_Vec mut* self, Addr value_ptr);


/// @brief Decreases a length by one and returns a pointer to the poped value.
/// 
/// @return the poped value.
AddrMut Cstl_Vec_pop(Cstl_Vec mut* self);


/// @brief Reserves exactly `additional_cap` as capacity.
/// 
/// @param additional_cap a capacity to reserve.
void Cstl_Vec_reserve_exact(Cstl_Vec mut* self, usize additional_cap);


/// @brief Reserves at least `additional_cap` as capacity.
/// 
/// @param additional_cap a minimal capacity to reserve.
void Cstl_Vec_reserve(Cstl_Vec mut* self, usize additional_cap);

/// @brief Retypes the [`Vec`]. Note that `self` should be an empty vector.
/// 
/// @param new_elem_size a new size of an element
void Cstl_Vec_retype(Cstl_Vec mut* self, usize new_elem_size);


/// @brief Clears the vector leaving capacity untouched.
void Cstl_Vec_clear(Cstl_Vec mut* self);


/// @brief Shrinks a capacity of the vector to `min_cap`.
///        Does nothing if `self->cap <= min_cap`.
/// 
/// @param min_cap the capatity to shrink to.
void Cstl_Vec_shrink_to(Cstl_Vec mut* self, usize min_cap);


/// @brief Shrinks a capacity of the vector to its length.
void Cstl_Vec_shrink_to_fit(Cstl_Vec mut* self);


/// @brief Computes address of a value at index `index`.
/// 
/// @param index 
/// @return pointer to that value.
AddrMut Cstl_Vec_get(Cstl_Vec const* self, usize index);


/// @brief Sets a value at index `index` to the value under `value_ptr`.
/// 
/// @param index value index.
/// @param value_ptr a pointer to the value to be set.
void Cstl_Vec_set(Cstl_Vec const* self, usize index, Addr value_ptr);


/// @brief Fills the vector with a value.
/// 
/// @param value_ptr a pointer to that value. Should point outside of
///        vector or should be aligned with vector values.
void Cstl_Vec_fill(Cstl_Vec const* self, Addr value_ptr);


/// @brief Gives [Slice] view of Vec.
Cstl_Slice Cstl_Vec_as_slice(Cstl_Vec const* self);


/// @brief Allocates memory for one element and returns its address.
AddrMut Cstl_Vec_push_in_place(Cstl_Vec mut* self);


/// @brief If `self` if `extremum_type`-heap places such extremum value
/// at the end of a vector and returns it's address.
AddrMut Cstl_Vec_extract(
    Cstl_Vec mut* self,
    Cstl_Ordering extremum_type,
    Cstl_Comparator cmp
);


/// @brief If `self` is min-heap places minimal value at the end of a vector
/// and returns it's address.
AddrMut Cstl_Vec_extract_min(
    Cstl_Vec mut* self, Cstl_Comparator cmp
);


/// @brief If `self` is max-heap places maximal value at the end of a vector
/// and returns it's address.
AddrMut Cstl_Vec_extract_max(
    Cstl_Vec mut* self, Cstl_Comparator cmp
);


/// @brief Slices the vector by [start, end).
Cstl_Slice Cstl_Vec_slice(Cstl_Vec mut* self, usize start, usize end);


/// @brief Slices the vector by [start, end) without checks.
Cstl_Slice Cstl_Vec_slice_unchecked(Cstl_Vec mut* self, usize start, usize end);



#define Cstl_declare_typed_Vec(Type) \
    typedef struct Cstl_Vec_##Type { \
        Type mut* ptr; \
        usize len; \
        usize cap; \
    } Cstl_Vec_##Type; \
    \
    Cstl_Vec_##Type Cstl_Vec_##Type##_new(void); \
    Cstl_Vec_##Type Cstl_Vec_##Type##_with_capacity(usize cap); \
    Cstl_Vec_##Type Cstl_Vec_##Type##_repeat(usize count, Type value); \
    Cstl_Vec_##Type Cstl_Vec_##Type##_from_typed_slice(Cstl_Slice_##Type src); \
    Cstl_Vec_##Type Cstl_Vec_##Type##_clone(Cstl_Vec_##Type const* self); \
    void Cstl_Vec_##Type##_clone_from(Cstl_Vec_##Type mut* self, Cstl_Vec_##Type const* src); \
    void Cstl_Vec_##Type##_free(Cstl_Vec_##Type const* self); \
    void Cstl__internal_Vec_##Type##_alloc(Cstl_Vec_##Type mut* self, usize new_cap); \
    usize Cstl__internal_Vec_##Type##_next_capacity(usize cur_cap); \
    void Cstl_Vec_##Type##_push(Cstl_Vec_##Type mut* self, Type value); \
    Type Cstl_Vec_##Type##_pop(Cstl_Vec_##Type mut* self); \
    void Cstl_Vec_##Type##_reserve_exact(Cstl_Vec_##Type mut* self, usize additional_cap); \
    void Cstl_Vec_##Type##_reserve(Cstl_Vec_##Type mut* self, usize additional_cap); \
    void Cstl_Vec_##Type##_clear(Cstl_Vec_##Type mut* self); \
    void Cstl_Vec_##Type##_shrink_to(Cstl_Vec_##Type mut* self, usize min_cap); \
    void Cstl_Vec_##Type##_shrink_to_fit(Cstl_Vec_##Type mut* self); \
    Type mut* Cstl_Vec_##Type##_get(Cstl_Vec_##Type const* self, usize index); \
    void Cstl_Vec_##Type##_set(Cstl_Vec_##Type const* self, usize index, Type value); \
    void Cstl_Vec_##Type##_fill(Cstl_Vec_##Type const* self, Type value); \
    Cstl_Slice Cstl_Vec_##Type##_as_slice(Cstl_Vec_##Type const* self); \
    Cstl_Slice_##Type Cstl_Vec_##Type##_as_typed_slice(Cstl_Vec_##Type const* self); \
    Type mut* Cstl_Vec_##Type##_push_in_place(Cstl_Vec_##Type mut* self); \
    Type Cstl_Vec_##Type##_extract(Cstl_Vec_##Type mut* self, Cstl_Ordering extremum_type); \
    Type Cstl_Vec_##Type##_extract_min(Cstl_Vec_##Type mut* self); \
    Type Cstl_Vec_##Type##_extract_max(Cstl_Vec_##Type mut* self); \
    Cstl_Slice Cstl_Vec_##Type##_slice(Cstl_Vec_##Type const* self, usize start, usize end); \
    Cstl_Slice_##Type Cstl_Vec_##Type##_slice_typed(Cstl_Vec_##Type const* self, usize start, usize end); \
    Cstl_Slice Cstl_Vec_##Type##_slice_unchecked(Cstl_Vec_##Type const* self, usize start, usize end); \
    Cstl_Slice_##Type Cstl_Vec_##Type##_slice_typed_unchecked(Cstl_Vec_##Type const* self, usize start, usize end); \
    Cstl_Vec Cstl_Vec_##Type##_to_untyped(Cstl_Vec_##Type const* self)

Cstl_declare_typed_Vec(u8);
Cstl_declare_typed_Vec(i8);
Cstl_declare_typed_Vec(u16);
Cstl_declare_typed_Vec(i16);
Cstl_declare_typed_Vec(u32);
Cstl_declare_typed_Vec(i32);
Cstl_declare_typed_Vec(u64);
Cstl_declare_typed_Vec(i64);
Cstl_declare_typed_Vec(usize);
Cstl_declare_typed_Vec(isize);
Cstl_declare_typed_Vec(char);
Cstl_declare_typed_Vec(bool);
Cstl_declare_typed_Vec(f32);
Cstl_declare_typed_Vec(f64);

#define Cstl_Vec_u8_DEFAULT (Cstl_Vec_u8) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_i8_DEFAULT (Cstl_Vec_i8) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_u16_DEFAULT (Cstl_Vec_u16) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_i16_DEFAULT (Cstl_Vec_i16) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_u32_DEFAULT (Cstl_Vec_u32) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_i32_DEFAULT (Cstl_Vec_i32) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_u64_DEFAULT (Cstl_Vec_u64) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_i64_DEFAULT (Cstl_Vec_i64) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_usize_DEFAULT (Cstl_Vec_usize) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_isize_DEFAULT (Cstl_Vec_isize) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_char_DEFAULT (Cstl_Vec_char) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_bool_DEFAULT (Cstl_Vec_bool) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_f32_DEFAULT (Cstl_Vec_f32) { .ptr = null_mut, .len = 0, .cap = 0 }
#define Cstl_Vec_f64_DEFAULT (Cstl_Vec_f64) { .ptr = null_mut, .len = 0, .cap = 0 }



#ifdef USING_NAMESPACE_CSTL

    #define Vec_new_meta Cstl_Vec_new_meta



    #define Vec_get_value Cstl_Vec_get_value
    #define Vec_from_elems Cstl_Vec_from_elems
    #define Vec Vec_from_elems
    #define Vec_push_value Cstl_Vec_push_value



    typedef Cstl_Vec Vec;

    #define Vec_new Cstl_Vec_new
    #define Vec_with_capacity Cstl_Vec_with_capacity
    #define Vec_from_slice Cstl_Vec_from_slice
    #define Vec_clone Cstl_Vec_clone    
    #define Vec_clone_from Cstl_Vec_clone_from
    #define Vec_free Cstl_Vec_free
    #define _internal_Vec_alloc Cstl__internal_Vec_alloc
    #define _internal_Vec_next_capacity Cstl__internal_Vec_next_capacity
    #define Vec_push Cstl_Vec_push
    #define Vec_pop Cstl_Vec_pop
    #define Vec_reserve_exact Cstl_Vec_reserve_exact
    #define Vec_reserve Cstl_Vec_reserve
    #define Vec_retype Cstl_Vec_retype
    #define Vec_clear Cstl_Vec_clear
    #define Vec_shrink_to Cstl_Vec_shrink_to
    #define Vec_shrink_to_fit Cstl_Vec_shrink_to_fit
    #define Vec_get Cstl_Vec_get
    #define Vec_set Cstl_Vec_set
    #define Vec_fill Cstl_Vec_fill
    #define Vec_as_slice Cstl_Vec_as_slice
    #define Vec_push_in_place Cstl_Vec_push_in_place
    #define Vec_extract Cstl_Vec_extract
    #define Vec_extract_min Cstl_Vec_extract_min
    #define Vec_extract_max Cstl_Vec_extract_ma
    #define Vec_slice Cstl_Vec_slice
    #define Vec_slice_unchecked Cstl_Vec_slice_unchecked



    typedef Cstl_Vec_u8 Vec_u8;
    typedef Cstl_Vec_i8 Vec_i8;
    typedef Cstl_Vec_u16 Vec_u16;
    typedef Cstl_Vec_i16 Vec_i16;
    typedef Cstl_Vec_u32 Vec_u32;
    typedef Cstl_Vec_i32 Vec_i32;
    typedef Cstl_Vec_u64 Vec_u64;
    typedef Cstl_Vec_i64 Vec_i64;
    typedef Cstl_Vec_usize Vec_usize;
    typedef Cstl_Vec_isize Vec_isize;
    typedef Cstl_Vec_char Vec_char;
    typedef Cstl_Vec_bool Vec_bool;
    typedef Cstl_Vec_f32 Vec_f32;
    typedef Cstl_Vec_f64 Vec_f64;

    #define Vec_u8_new Cstl_Vec_u8_new
    #define Vec_u8_with_capacity Cstl_Vec_u8_with_capacity
    #define Vec_u8_repeat Cstl_Vec_u8_repeat
    #define Vec_u8_from_typed_slice Cstl_Vec_u8_from_typed_slice
    #define Vec_u8_clone Cstl_Vec_u8_clone
    #define Vec_u8_clone_from Cstl_Vec_u8_clone_from
    #define Vec_u8_free Cstl_Vec_u8_free
    #define _internal_Vec_u8_alloc Cstl__internal_Vec_u8_alloc
    #define _internal_Vec_u8_next_capacity Ctl__internal_Vec_u8_next_capacity
    #define Vec_u8_push Cstl_Vec_u8_push
    #define Vec_u8_pop Cstl_Vec_u8_pop
    #define Vec_u8_reserve_exact Cstl_Vec_u8_reserve_exact
    #define Vec_u8_reserve Cstl_Vec_u8_reserve
    #define Vec_u8_clear Cstl_Vec_u8_clear
    #define Vec_u8_shrink_to Cstl_Vec_u8_shrink_to
    #define Vec_u8_shrink_to_fit Cstl_Vec_u8_shrink_to_fit
    #define Vec_u8_get Cstl_Vec_u8_get
    #define Vec_u8_set Cstl_Vec_u8_set
    #define Vec_u8_fill Cstl_Vec_u8_fill
    #define Vec_u8_as_slice Cstl_Vec_u8_as_slice
    #define Vec_u8_as_typed_slice Cstl_Vec_u8_as_typed_slice
    #define Vec_u8_push_in_place Cstl_Vec_u8_push_in_place
    #define Vec_u8_extract Cstl_Vec_u8_extract
    #define Vec_u8_extract_min Cstl_Vec_u8_extract_min
    #define Vec_u8_extract_max Cstl_Vec_u8_extract_max
    #define Vec_u8_slice Cstl_Vec_u8_slice
    #define Vec_u8_slice_typed Cstl_Vec_u8_slice_typed
    #define Vec_u8_slice_unchecked Cstl_Vec_u8_slice_unchecked
    #define Vec_u8_slice_typed_unchecked Cstl_Vec_u8_slice_typed_unchecked
    #define Vec_u8_to_untyped Cstl_Vec_u8_to_untyped

    #define Vec_i8_new Cstl_Vec_i8_new
    #define Vec_i8_with_capacity Cstl_Vec_i8_with_capacity
    #define Vec_i8_repeat Cstl_Vec_i8_repeat
    #define Vec_i8_from_typed_slice Cstl_Vec_i8_from_typed_slice
    #define Vec_i8_clone Cstl_Vec_i8_clone
    #define Vec_i8_clone_from Cstl_Vec_i8_clone_from
    #define Vec_i8_free Cstl_Vec_i8_free
    #define _internal_Vec_i8_alloc Cstl__internal_Vec_i8_alloc
    #define _internal_Vec_i8_next_capacity Ctl__internal_Vec_i8_next_capacity
    #define Vec_i8_push Cstl_Vec_i8_push
    #define Vec_i8_pop Cstl_Vec_i8_pop
    #define Vec_i8_reserve_exact Cstl_Vec_i8_reserve_exact
    #define Vec_i8_reserve Cstl_Vec_i8_reserve
    #define Vec_i8_clear Cstl_Vec_i8_clear
    #define Vec_i8_shrink_to Cstl_Vec_i8_shrink_to
    #define Vec_i8_shrink_to_fit Cstl_Vec_i8_shrink_to_fit
    #define Vec_i8_get Cstl_Vec_i8_get
    #define Vec_i8_set Cstl_Vec_i8_set
    #define Vec_i8_fill Cstl_Vec_i8_fill
    #define Vec_i8_as_slice Cstl_Vec_i8_as_slice
    #define Vec_i8_as_typed_slice Cstl_Vec_i8_as_typed_slice
    #define Vec_i8_push_in_place Cstl_Vec_i8_push_in_place
    #define Vec_i8_extract Cstl_Vec_i8_extract
    #define Vec_i8_extract_min Cstl_Vec_i8_extract_min
    #define Vec_i8_extract_max Cstl_Vec_i8_extract_max
    #define Vec_i8_slice Cstl_Vec_i8_slice
    #define Vec_i8_slice_typed Cstl_Vec_i8_slice_typed
    #define Vec_i8_slice_unchecked Cstl_Vec_i8_slice_unchecked
    #define Vec_i8_slice_typed_unchecked Cstl_Vec_i8_slice_typed_unchecked
    #define Vec_i8_to_untyped Cstl_Vec_i8_to_untyped

    #define Vec_u16_new Cstl_Vec_u16_new
    #define Vec_u16_with_capacity Cstl_Vec_u16_with_capacity
    #define Vec_u16_repeat Cstl_Vec_u16_repeat
    #define Vec_u16_from_typed_slice Cstl_Vec_u16_from_typed_slice
    #define Vec_u16_clone Cstl_Vec_u16_clone
    #define Vec_u16_clone_from Cstl_Vec_u16_clone_from
    #define Vec_u16_free Cstl_Vec_u16_free
    #define _internal_Vec_u16_alloc Cstl__internal_Vec_u16_alloc
    #define _internal_Vec_u16_next_capacity Ctl__internal_Vec_u16_next_capacity
    #define Vec_u16_push Cstl_Vec_u16_push
    #define Vec_u16_pop Cstl_Vec_u16_pop
    #define Vec_u16_reserve_exact Cstl_Vec_u16_reserve_exact
    #define Vec_u16_reserve Cstl_Vec_u16_reserve
    #define Vec_u16_clear Cstl_Vec_u16_clear
    #define Vec_u16_shrink_to Cstl_Vec_u16_shrink_to
    #define Vec_u16_shrink_to_fit Cstl_Vec_u16_shrink_to_fit
    #define Vec_u16_get Cstl_Vec_u16_get
    #define Vec_u16_set Cstl_Vec_u16_set
    #define Vec_u16_fill Cstl_Vec_u16_fill
    #define Vec_u16_as_slice Cstl_Vec_u16_as_slice
    #define Vec_u16_as_typed_slice Cstl_Vec_u16_as_typed_slice
    #define Vec_u16_push_in_place Cstl_Vec_u16_push_in_place
    #define Vec_u16_extract Cstl_Vec_u16_extract
    #define Vec_u16_extract_min Cstl_Vec_u16_extract_min
    #define Vec_u16_extract_max Cstl_Vec_u16_extract_max
    #define Vec_u16_slice Cstl_Vec_u16_slice
    #define Vec_u16_slice_typed Cstl_Vec_u16_slice_typed
    #define Vec_u16_slice_unchecked Cstl_Vec_u16_slice_unchecked
    #define Vec_u16_slice_typed_unchecked Cstl_Vec_u16_slice_typed_unchecked
    #define Vec_u16_to_untyped Cstl_Vec_u16_to_untyped

    #define Vec_i16_new Cstl_Vec_i16_new
    #define Vec_i16_with_capacity Cstl_Vec_i16_with_capacity
    #define Vec_i16_repeat Cstl_Vec_i16_repeat
    #define Vec_i16_from_typed_slice Cstl_Vec_i16_from_typed_slice
    #define Vec_i16_clone Cstl_Vec_i16_clone
    #define Vec_i16_clone_from Cstl_Vec_i16_clone_from
    #define Vec_i16_free Cstl_Vec_i16_free
    #define _internal_Vec_i16_alloc Cstl__internal_Vec_i16_alloc
    #define _internal_Vec_i16_next_capacity Ctl__internal_Vec_i16_next_capacity
    #define Vec_i16_push Cstl_Vec_i16_push
    #define Vec_i16_pop Cstl_Vec_i16_pop
    #define Vec_i16_reserve_exact Cstl_Vec_i16_reserve_exact
    #define Vec_i16_reserve Cstl_Vec_i16_reserve
    #define Vec_i16_clear Cstl_Vec_i16_clear
    #define Vec_i16_shrink_to Cstl_Vec_i16_shrink_to
    #define Vec_i16_shrink_to_fit Cstl_Vec_i16_shrink_to_fit
    #define Vec_i16_get Cstl_Vec_i16_get
    #define Vec_i16_set Cstl_Vec_i16_set
    #define Vec_i16_fill Cstl_Vec_i16_fill
    #define Vec_i16_as_slice Cstl_Vec_i16_as_slice
    #define Vec_i16_as_typed_slice Cstl_Vec_i16_as_typed_slice
    #define Vec_i16_push_in_place Cstl_Vec_i16_push_in_place
    #define Vec_i16_extract Cstl_Vec_i16_extract
    #define Vec_i16_extract_min Cstl_Vec_i16_extract_min
    #define Vec_i16_extract_max Cstl_Vec_i16_extract_max
    #define Vec_i16_slice Cstl_Vec_i16_slice
    #define Vec_i16_slice_typed Cstl_Vec_i16_slice_typed
    #define Vec_i16_slice_unchecked Cstl_Vec_i16_slice_unchecked
    #define Vec_i16_slice_typed_unchecked Cstl_Vec_i16_slice_typed_unchecked
    #define Vec_i16_to_untyped Cstl_Vec_i16_to_untyped

    #define Vec_u32_new Cstl_Vec_u32_new
    #define Vec_u32_with_capacity Cstl_Vec_u32_with_capacity
    #define Vec_u32_repeat Cstl_Vec_u32_repeat
    #define Vec_u32_from_typed_slice Cstl_Vec_u32_from_typed_slice
    #define Vec_u32_clone Cstl_Vec_u32_clone
    #define Vec_u32_clone_from Cstl_Vec_u32_clone_from
    #define Vec_u32_free Cstl_Vec_u32_free
    #define _internal_Vec_u32_alloc Cstl__internal_Vec_u32_alloc
    #define _internal_Vec_u32_next_capacity Ctl__internal_Vec_u32_next_capacity
    #define Vec_u32_push Cstl_Vec_u32_push
    #define Vec_u32_pop Cstl_Vec_u32_pop
    #define Vec_u32_reserve_exact Cstl_Vec_u32_reserve_exact
    #define Vec_u32_reserve Cstl_Vec_u32_reserve
    #define Vec_u32_clear Cstl_Vec_u32_clear
    #define Vec_u32_shrink_to Cstl_Vec_u32_shrink_to
    #define Vec_u32_shrink_to_fit Cstl_Vec_u32_shrink_to_fit
    #define Vec_u32_get Cstl_Vec_u32_get
    #define Vec_u32_set Cstl_Vec_u32_set
    #define Vec_u32_fill Cstl_Vec_u32_fill
    #define Vec_u32_as_slice Cstl_Vec_u32_as_slice
    #define Vec_u32_as_typed_slice Cstl_Vec_u32_as_typed_slice
    #define Vec_u32_push_in_place Cstl_Vec_u32_push_in_place
    #define Vec_u32_extract Cstl_Vec_u32_extract
    #define Vec_u32_extract_min Cstl_Vec_u32_extract_min
    #define Vec_u32_extract_max Cstl_Vec_u32_extract_max
    #define Vec_u32_slice Cstl_Vec_u32_slice
    #define Vec_u32_slice_typed Cstl_Vec_u32_slice_typed
    #define Vec_u32_slice_unchecked Cstl_Vec_u32_slice_unchecked
    #define Vec_u32_slice_typed_unchecked Cstl_Vec_u32_slice_typed_unchecked
    #define Vec_u32_to_untyped Cstl_Vec_u32_to_untyped

    #define Vec_i32_new Cstl_Vec_i32_new
    #define Vec_i32_with_capacity Cstl_Vec_i32_with_capacity
    #define Vec_i32_repeat Cstl_Vec_i32_repeat
    #define Vec_i32_from_typed_slice Cstl_Vec_i32_from_typed_slice
    #define Vec_i32_clone Cstl_Vec_i32_clone
    #define Vec_i32_clone_from Cstl_Vec_i32_clone_from
    #define Vec_i32_free Cstl_Vec_i32_free
    #define _internal_Vec_i32_alloc Cstl__internal_Vec_i32_alloc
    #define _internal_Vec_i32_next_capacity Ctl__internal_Vec_i32_next_capacity
    #define Vec_i32_push Cstl_Vec_i32_push
    #define Vec_i32_pop Cstl_Vec_i32_pop
    #define Vec_i32_reserve_exact Cstl_Vec_i32_reserve_exact
    #define Vec_i32_reserve Cstl_Vec_i32_reserve
    #define Vec_i32_clear Cstl_Vec_i32_clear
    #define Vec_i32_shrink_to Cstl_Vec_i32_shrink_to
    #define Vec_i32_shrink_to_fit Cstl_Vec_i32_shrink_to_fit
    #define Vec_i32_get Cstl_Vec_i32_get
    #define Vec_i32_set Cstl_Vec_i32_set
    #define Vec_i32_fill Cstl_Vec_i32_fill
    #define Vec_i32_as_slice Cstl_Vec_i32_as_slice
    #define Vec_i32_as_typed_slice Cstl_Vec_i32_as_typed_slice
    #define Vec_i32_push_in_place Cstl_Vec_i32_push_in_place
    #define Vec_i32_extract Cstl_Vec_i32_extract
    #define Vec_i32_extract_min Cstl_Vec_i32_extract_min
    #define Vec_i32_extract_max Cstl_Vec_i32_extract_max
    #define Vec_i32_slice Cstl_Vec_i32_slice
    #define Vec_i32_slice_typed Cstl_Vec_i32_slice_typed
    #define Vec_i32_slice_unchecked Cstl_Vec_i32_slice_unchecked
    #define Vec_i32_slice_typed_unchecked Cstl_Vec_i32_slice_typed_unchecked
    #define Vec_i32_to_untyped Cstl_Vec_i32_to_untyped

    #define Vec_u64_new Cstl_Vec_u64_new
    #define Vec_u64_with_capacity Cstl_Vec_u64_with_capacity
    #define Vec_u64_repeat Cstl_Vec_u64_repeat
    #define Vec_u64_from_typed_slice Cstl_Vec_u64_from_typed_slice
    #define Vec_u64_clone Cstl_Vec_u64_clone
    #define Vec_u64_clone_from Cstl_Vec_u64_clone_from
    #define Vec_u64_free Cstl_Vec_u64_free
    #define _internal_Vec_u64_alloc Cstl__internal_Vec_u64_alloc
    #define _internal_Vec_u64_next_capacity Ctl__internal_Vec_u64_next_capacity
    #define Vec_u64_push Cstl_Vec_u64_push
    #define Vec_u64_pop Cstl_Vec_u64_pop
    #define Vec_u64_reserve_exact Cstl_Vec_u64_reserve_exact
    #define Vec_u64_reserve Cstl_Vec_u64_reserve
    #define Vec_u64_clear Cstl_Vec_u64_clear
    #define Vec_u64_shrink_to Cstl_Vec_u64_shrink_to
    #define Vec_u64_shrink_to_fit Cstl_Vec_u64_shrink_to_fit
    #define Vec_u64_get Cstl_Vec_u64_get
    #define Vec_u64_set Cstl_Vec_u64_set
    #define Vec_u64_fill Cstl_Vec_u64_fill
    #define Vec_u64_as_slice Cstl_Vec_u64_as_slice
    #define Vec_u64_as_typed_slice Cstl_Vec_u64_as_typed_slice
    #define Vec_u64_push_in_place Cstl_Vec_u64_push_in_place
    #define Vec_u64_extract Cstl_Vec_u64_extract
    #define Vec_u64_extract_min Cstl_Vec_u64_extract_min
    #define Vec_u64_extract_max Cstl_Vec_u64_extract_max
    #define Vec_u64_slice Cstl_Vec_u64_slice
    #define Vec_u64_slice_typed Cstl_Vec_u64_slice_typed
    #define Vec_u64_slice_unchecked Cstl_Vec_u64_slice_unchecked
    #define Vec_u64_slice_typed_unchecked Cstl_Vec_u64_slice_typed_unchecked
    #define Vec_u64_to_untyped Cstl_Vec_u64_to_untyped

    #define Vec_i64_new Cstl_Vec_i64_new
    #define Vec_i64_with_capacity Cstl_Vec_i64_with_capacity
    #define Vec_i64_repeat Cstl_Vec_i64_repeat
    #define Vec_i64_from_typed_slice Cstl_Vec_i64_from_typed_slice
    #define Vec_i64_clone Cstl_Vec_i64_clone
    #define Vec_i64_clone_from Cstl_Vec_i64_clone_from
    #define Vec_i64_free Cstl_Vec_i64_free
    #define _internal_Vec_i64_alloc Cstl__internal_Vec_i64_alloc
    #define _internal_Vec_i64_next_capacity Ctl__internal_Vec_i64_next_capacity
    #define Vec_i64_push Cstl_Vec_i64_push
    #define Vec_i64_pop Cstl_Vec_i64_pop
    #define Vec_i64_reserve_exact Cstl_Vec_i64_reserve_exact
    #define Vec_i64_reserve Cstl_Vec_i64_reserve
    #define Vec_i64_clear Cstl_Vec_i64_clear
    #define Vec_i64_shrink_to Cstl_Vec_i64_shrink_to
    #define Vec_i64_shrink_to_fit Cstl_Vec_i64_shrink_to_fit
    #define Vec_i64_get Cstl_Vec_i64_get
    #define Vec_i64_set Cstl_Vec_i64_set
    #define Vec_i64_fill Cstl_Vec_i64_fill
    #define Vec_i64_as_slice Cstl_Vec_i64_as_slice
    #define Vec_i64_as_typed_slice Cstl_Vec_i64_as_typed_slice
    #define Vec_i64_push_in_place Cstl_Vec_i64_push_in_place
    #define Vec_i64_extract Cstl_Vec_i64_extract
    #define Vec_i64_extract_min Cstl_Vec_i64_extract_min
    #define Vec_i64_extract_max Cstl_Vec_i64_extract_max
    #define Vec_i64_slice Cstl_Vec_i64_slice
    #define Vec_i64_slice_typed Cstl_Vec_i64_slice_typed
    #define Vec_i64_slice_unchecked Cstl_Vec_i64_slice_unchecked
    #define Vec_i64_slice_typed_unchecked Cstl_Vec_i64_slice_typed_unchecked
    #define Vec_i64_to_untyped Cstl_Vec_i64_to_untyped

    #define Vec_usize_new Cstl_Vec_usize_new
    #define Vec_usize_with_capacity Cstl_Vec_usize_with_capacity
    #define Vec_usize_repeat Cstl_Vec_usize_repeat
    #define Vec_usize_from_typed_slice Cstl_Vec_usize_from_typed_slice
    #define Vec_usize_clone Cstl_Vec_usize_clone
    #define Vec_usize_clone_from Cstl_Vec_usize_clone_from
    #define Vec_usize_free Cstl_Vec_usize_free
    #define _internal_Vec_usize_alloc Cstl__internal_Vec_usize_alloc
    #define _internal_Vec_usize_next_capacity Ctl__internal_Vec_usize_next_capacity
    #define Vec_usize_push Cstl_Vec_usize_push
    #define Vec_usize_pop Cstl_Vec_usize_pop
    #define Vec_usize_reserve_exact Cstl_Vec_usize_reserve_exact
    #define Vec_usize_reserve Cstl_Vec_usize_reserve
    #define Vec_usize_clear Cstl_Vec_usize_clear
    #define Vec_usize_shrink_to Cstl_Vec_usize_shrink_to
    #define Vec_usize_shrink_to_fit Cstl_Vec_usize_shrink_to_fit
    #define Vec_usize_get Cstl_Vec_usize_get
    #define Vec_usize_set Cstl_Vec_usize_set
    #define Vec_usize_fill Cstl_Vec_usize_fill
    #define Vec_usize_as_slice Cstl_Vec_usize_as_slice
    #define Vec_usize_as_typed_slice Cstl_Vec_usize_as_typed_slice
    #define Vec_usize_push_in_place Cstl_Vec_usize_push_in_place
    #define Vec_usize_extract Cstl_Vec_usize_extract
    #define Vec_usize_extract_min Cstl_Vec_usize_extract_min
    #define Vec_usize_extract_max Cstl_Vec_usize_extract_max
    #define Vec_usize_slice Cstl_Vec_usize_slice
    #define Vec_usize_slice_typed Cstl_Vec_usize_slice_typed
    #define Vec_usize_slice_unchecked Cstl_Vec_usize_slice_unchecked
    #define Vec_usize_slice_typed_unchecked Cstl_Vec_usize_slice_typed_unchecked
    #define Vec_usize_to_untyped Cstl_Vec_usize_to_untyped

    #define Vec_isize_new Cstl_Vec_isize_new
    #define Vec_isize_with_capacity Cstl_Vec_isize_with_capacity
    #define Vec_isize_repeat Cstl_Vec_isize_repeat
    #define Vec_isize_from_typed_slice Cstl_Vec_isize_from_typed_slice
    #define Vec_isize_clone Cstl_Vec_isize_clone
    #define Vec_isize_clone_from Cstl_Vec_isize_clone_from
    #define Vec_isize_free Cstl_Vec_isize_free
    #define _internal_Vec_isize_alloc Cstl__internal_Vec_isize_alloc
    #define _internal_Vec_isize_next_capacity Ctl__internal_Vec_isize_next_capacity
    #define Vec_isize_push Cstl_Vec_isize_push
    #define Vec_isize_pop Cstl_Vec_isize_pop
    #define Vec_isize_reserve_exact Cstl_Vec_isize_reserve_exact
    #define Vec_isize_reserve Cstl_Vec_isize_reserve
    #define Vec_isize_clear Cstl_Vec_isize_clear
    #define Vec_isize_shrink_to Cstl_Vec_isize_shrink_to
    #define Vec_isize_shrink_to_fit Cstl_Vec_isize_shrink_to_fit
    #define Vec_isize_get Cstl_Vec_isize_get
    #define Vec_isize_set Cstl_Vec_isize_set
    #define Vec_isize_fill Cstl_Vec_isize_fill
    #define Vec_isize_as_slice Cstl_Vec_isize_as_slice
    #define Vec_isize_as_typed_slice Cstl_Vec_isize_as_typed_slice
    #define Vec_isize_push_in_place Cstl_Vec_isize_push_in_place
    #define Vec_isize_extract Cstl_Vec_isize_extract
    #define Vec_isize_extract_min Cstl_Vec_isize_extract_min
    #define Vec_isize_extract_max Cstl_Vec_isize_extract_max
    #define Vec_isize_slice Cstl_Vec_isize_slice
    #define Vec_isize_slice_typed Cstl_Vec_isize_slice_typed
    #define Vec_isize_slice_unchecked Cstl_Vec_isize_slice_unchecked
    #define Vec_isize_slice_typed_unchecked Cstl_Vec_isize_slice_typed_unchecked
    #define Vec_isize_to_untyped Cstl_Vec_isize_to_untyped

    #define Vec_char_new Cstl_Vec_char_new
    #define Vec_char_with_capacity Cstl_Vec_char_with_capacity
    #define Vec_char_repeat Cstl_Vec_char_repeat
    #define Vec_char_from_typed_slice Cstl_Vec_char_from_typed_slice
    #define Vec_char_clone Cstl_Vec_char_clone
    #define Vec_char_clone_from Cstl_Vec_char_clone_from
    #define Vec_char_free Cstl_Vec_char_free
    #define _internal_Vec_char_alloc Cstl__internal_Vec_char_alloc
    #define _internal_Vec_char_next_capacity Ctl__internal_Vec_char_next_capacity
    #define Vec_char_push Cstl_Vec_char_push
    #define Vec_char_pop Cstl_Vec_char_pop
    #define Vec_char_reserve_exact Cstl_Vec_char_reserve_exact
    #define Vec_char_reserve Cstl_Vec_char_reserve
    #define Vec_char_clear Cstl_Vec_char_clear
    #define Vec_char_shrink_to Cstl_Vec_char_shrink_to
    #define Vec_char_shrink_to_fit Cstl_Vec_char_shrink_to_fit
    #define Vec_char_get Cstl_Vec_char_get
    #define Vec_char_set Cstl_Vec_char_set
    #define Vec_char_fill Cstl_Vec_char_fill
    #define Vec_char_as_slice Cstl_Vec_char_as_slice
    #define Vec_char_as_typed_slice Cstl_Vec_char_as_typed_slice
    #define Vec_char_push_in_place Cstl_Vec_char_push_in_place
    #define Vec_char_extract Cstl_Vec_char_extract
    #define Vec_char_extract_min Cstl_Vec_char_extract_min
    #define Vec_char_extract_max Cstl_Vec_char_extract_max
    #define Vec_char_slice Cstl_Vec_char_slice
    #define Vec_char_slice_typed Cstl_Vec_char_slice_typed
    #define Vec_char_slice_unchecked Cstl_Vec_char_slice_unchecked
    #define Vec_char_slice_typed_unchecked Cstl_Vec_char_slice_typed_unchecked
    #define Vec_char_to_untyped Cstl_Vec_char_to_untyped

    #define Vec_bool_new Cstl_Vec_bool_new
    #define Vec_bool_with_capacity Cstl_Vec_bool_with_capacity
    #define Vec_bool_repeat Cstl_Vec_bool_repeat
    #define Vec_bool_from_typed_slice Cstl_Vec_bool_from_typed_slice
    #define Vec_bool_clone Cstl_Vec_bool_clone
    #define Vec_bool_clone_from Cstl_Vec_bool_clone_from
    #define Vec_bool_free Cstl_Vec_bool_free
    #define _internal_Vec_bool_alloc Cstl__internal_Vec_bool_alloc
    #define _internal_Vec_bool_next_capacity Ctl__internal_Vec_bool_next_capacity
    #define Vec_bool_push Cstl_Vec_bool_push
    #define Vec_bool_pop Cstl_Vec_bool_pop
    #define Vec_bool_reserve_exact Cstl_Vec_bool_reserve_exact
    #define Vec_bool_reserve Cstl_Vec_bool_reserve
    #define Vec_bool_clear Cstl_Vec_bool_clear
    #define Vec_bool_shrink_to Cstl_Vec_bool_shrink_to
    #define Vec_bool_shrink_to_fit Cstl_Vec_bool_shrink_to_fit
    #define Vec_bool_get Cstl_Vec_bool_get
    #define Vec_bool_set Cstl_Vec_bool_set
    #define Vec_bool_fill Cstl_Vec_bool_fill
    #define Vec_bool_as_slice Cstl_Vec_bool_as_slice
    #define Vec_bool_as_typed_slice Cstl_Vec_bool_as_typed_slice
    #define Vec_bool_push_in_place Cstl_Vec_bool_push_in_place
    #define Vec_bool_extract Cstl_Vec_bool_extract
    #define Vec_bool_extract_min Cstl_Vec_bool_extract_min
    #define Vec_bool_extract_max Cstl_Vec_bool_extract_max
    #define Vec_bool_slice Cstl_Vec_bool_slice
    #define Vec_bool_slice_typed Cstl_Vec_bool_slice_typed
    #define Vec_bool_slice_unchecked Cstl_Vec_bool_slice_unchecked
    #define Vec_bool_slice_typed_unchecked Cstl_Vec_bool_slice_typed_unchecked
    #define Vec_bool_to_untyped Cstl_Vec_bool_to_untyped

    #define Vec_f32_new Cstl_Vec_f32_new
    #define Vec_f32_with_capacity Cstl_Vec_f32_with_capacity
    #define Vec_f32_repeat Cstl_Vec_f32_repeat
    #define Vec_f32_from_typed_slice Cstl_Vec_f32_from_typed_slice
    #define Vec_f32_clone Cstl_Vec_f32_clone
    #define Vec_f32_clone_from Cstl_Vec_f32_clone_from
    #define Vec_f32_free Cstl_Vec_f32_free
    #define _internal_Vec_f32_alloc Cstl__internal_Vec_f32_alloc
    #define _internal_Vec_f32_next_capacity Ctl__internal_Vec_f32_next_capacity
    #define Vec_f32_push Cstl_Vec_f32_push
    #define Vec_f32_pop Cstl_Vec_f32_pop
    #define Vec_f32_reserve_exact Cstl_Vec_f32_reserve_exact
    #define Vec_f32_reserve Cstl_Vec_f32_reserve
    #define Vec_f32_clear Cstl_Vec_f32_clear
    #define Vec_f32_shrink_to Cstl_Vec_f32_shrink_to
    #define Vec_f32_shrink_to_fit Cstl_Vec_f32_shrink_to_fit
    #define Vec_f32_get Cstl_Vec_f32_get
    #define Vec_f32_set Cstl_Vec_f32_set
    #define Vec_f32_fill Cstl_Vec_f32_fill
    #define Vec_f32_as_slice Cstl_Vec_f32_as_slice
    #define Vec_f32_as_typed_slice Cstl_Vec_f32_as_typed_slice
    #define Vec_f32_push_in_place Cstl_Vec_f32_push_in_place
    #define Vec_f32_extract Cstl_Vec_f32_extract
    #define Vec_f32_extract_min Cstl_Vec_f32_extract_min
    #define Vec_f32_extract_max Cstl_Vec_f32_extract_max
    #define Vec_f32_slice Cstl_Vec_f32_slice
    #define Vec_f32_slice_typed Cstl_Vec_f32_slice_typed
    #define Vec_f32_slice_unchecked Cstl_Vec_f32_slice_unchecked
    #define Vec_f32_slice_typed_unchecked Cstl_Vec_f32_slice_typed_unchecked
    #define Vec_f32_to_untyped Cstl_Vec_f32_to_untyped

    #define Vec_f64_new Cstl_Vec_f64_new
    #define Vec_f64_with_capacity Cstl_Vec_f64_with_capacity
    #define Vec_f64_repeat Cstl_f64_repeat
    #define Vec_f64_from_typed_slice Cstl_Vec_f64_from_typed_slice
    #define Vec_f64_clone Cstl_Vec_f64_clone
    #define Vec_f64_clone_from Cstl_Vec_f64_clone_from
    #define Vec_f64_free Cstl_Vec_f64_free
    #define _internal_Vec_f64_alloc Cstl__internal_Vec_f64_alloc
    #define _internal_Vec_f64_next_capacity Ctl__internal_Vec_f64_next_capacity
    #define Vec_f64_push Cstl_Vec_f64_push
    #define Vec_f64_pop Cstl_Vec_f64_pop
    #define Vec_f64_reserve_exact Cstl_Vec_f64_reserve_exact
    #define Vec_f64_reserve Cstl_Vec_f64_reserve
    #define Vec_f64_clear Cstl_Vec_f64_clear
    #define Vec_f64_shrink_to Cstl_Vec_f64_shrink_to
    #define Vec_f64_shrink_to_fit Cstl_Vec_f64_shrink_to_fit
    #define Vec_f64_get Cstl_Vec_f64_get
    #define Vec_f64_set Cstl_Vec_f64_set
    #define Vec_f64_fill Cstl_Vec_f64_fill
    #define Vec_f64_as_slice Cstl_Vec_f64_as_slice
    #define Vec_f64_as_typed_slice Cstl_Vec_f64_as_typed_slice
    #define Vec_f64_push_in_place Cstl_Vec_f64_push_in_place
    #define Vec_f64_extract Cstl_Vec_f64_extract
    #define Vec_f64_extract_min Cstl_Vec_f64_extract_min
    #define Vec_f64_extract_max Cstl_Vec_f64_extract_max
    #define Vec_f64_slice Cstl_Vec_f64_slice
    #define Vec_f64_slice_typed Cstl_Vec_f64_slice_typed
    #define Vec_f64_slice_unchecked Cstl_Vec_f64_slice_unchecked
    #define Vec_f64_slice_typed_unchecked Cstl_Vec_f64_slice_typed_unchecked
    #define Vec_f64_to_untyped Cstl_Vec_f64_to_untyped

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_VEC_H_