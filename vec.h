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
typedef struct {
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



#ifdef USING_NAMESPACE_CSTL

    #define Vec_new_meta(elem_size) \
        Cstl_Vec_new_meta(elem_size)



    #define Vec_get_value(self, Type, index) \
        Cstl_Vec_get_value(self, Type, index)

    #define Vec_from_elems(Type, elems...) \
        Cstl_Vec_from_elems(Type, elems)

    #define Vec_push_value(self, Type, value) \
        Cstl_Vec_push_value(self, Type, value)



    /// @brief Dynamic array implementation in C.
    typedef Cstl_Vec Vec;

    #define Vec_new(elem_size) \
        Cstl_Vec_new(elem_size)

    #define Vec_with_capacity(cap, elem_size) \
        Cstl_Vec_with_capacity(cap, elem_size)

    #define Vec_from_slice(src) \
        Cstl_Vec_from_slice(src)

    #define Vec_clone(self) \
        Cstl_Vec_clone(self)
    
    #define Vec_clone_from(self, src) \
        Cstl_Vec_clone_from(self, src)

    #define Vec_free(self) \
        Cstl_Vec_free(self)

    #define _internal_Vec_alloc(self, new_cap)  \
        Cstl__internal_Vec_alloc(self, new_cap)

    #define _internal_Vec_next_capacity(cur_cap, elem_size) \
        Cstl__internal_Vec_next_capacity(cur_cap, elem_size)

    #define Vec_push(self, value_ptr) \
        Cstl_Vec_push(self, value_ptr)

    #define Vec_pop(self) \
        Cstl_Vec_pop(self)

    #define Vec_reserve_exact(self, additional_cap) \
        Cstl_Vec_reserve_exact(self, additional_cap)


    #define Vec_reserve(self, additional_cap) \
        Cstl_Vec_reserve(self, additional_cap)

    #define Vec_clear(self) \
        Cstl_Vec_clear(self)

    #define Vec_shrink_to(self, min_cap) \
        Cstl_Vec_shrink_to(self, min_cap)

    #define Vec_shrink_to_fit(self) \
        Cstl_Vec_shrink_to_fit(self)

    #define Vec_get(self, index) \
        Cstl_Vec_get(self, index)

    #define Vec_set(self, index, value_ptr) \
        Cstl_Vec_set(self, index, value_ptr)

    #define Vec_fill(self, value_ptr) \
        Cstl_Vec_fill(self, value_ptr)

    #define Vec_as_slice(self) \
        Cstl_Vec_as_slice(self)

    #define Vec_push_in_place(self) \
        Cstl_Vec_push_in_place(self)

    #define Vec_extract(self, extremum_type, cmp) \
        Cstl_Vec_extract(self, extremum_type, cmp)

    #define Vec_extract_min(self, cmp) \
        Cstl_Vec_extract_min(self, cmp)

    #define Vec_extract_max(self, cmp) \
        Cstl_Vec_extract_max(self, cmp)

    #define Vec_slice(self, start, end) \
    
    #define Vec_slice_unchecked(self, start, end) \
        Cstl_Vec_slice_unchecked(self, start, end)

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_VEC_H_