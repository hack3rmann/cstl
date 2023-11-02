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
    (*(Type*)Cstl_Vec_get((self), (index)))



typedef enum {
    Cstl_VecMetaFlags_Default = (u16) 0
} Cstl_VecMetaFlags;

Cstl_VecMetaFlags Cstl_VecMetaFlags_new(usize elem_size);



/// @brief Dynamic array implementation in C.
typedef struct {
    AddrMut ptr;
    usize len;
    Cstl_CollectionMetaData meta;
    usize cap;
} Cstl_Vec;


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
void Cstl_Vec_clone_from(Cstl_Vec* self, Cstl_Vec const* src);


/// @brief Frees a memory of the vector.
void Cstl_Vec_free(Cstl_Vec const* self);


void Cstl__internal_Vec_alloc(Cstl_Vec* self, usize new_cap);


usize Cstl__internal_Vec_next_capacity(usize cur_cap, usize elem_size);


/// @brief Pushes the value to the end of the vector.
/// 
/// @param value_ptr a pointer to the value.
void Cstl_Vec_push(Cstl_Vec* self, Addr value_ptr);


/// @brief Decreases a length by one and returns a pointer to the poped value.
/// 
/// @return the poped value.
void* Cstl_Vec_pop(Cstl_Vec* self);


/// @brief Reserves exactly `additional_cap` as capacity.
/// 
/// @param additional_cap a capacity to reserve.
void Cstl_Vec_reserve_exact(Cstl_Vec* self, usize additional_cap);


/// @brief Reserves at least `additional_cap` as capacity.
/// 
/// @param additional_cap a minimal capacity to reserve.
void Cstl_Vec_reserve(Cstl_Vec* self, usize additional_cap);


/// @brief Clears the vector leaving capacity untouched.
void Cstl_Vec_clear(Cstl_Vec* self);


/// @brief Shrinks a capacity of the vector to `min_cap`.
///        Does nothing if `self->cap <= min_cap`.
/// 
/// @param min_cap the capatity to shrink to.
void Cstl_Vec_shrink_to(Cstl_Vec* self, usize min_cap);


/// @brief Shrinks a capacity of the vector to its length.
void Cstl_Vec_shrink_to_fit(Cstl_Vec* self);


/// @brief Computes address of a value at index `index`.
/// 
/// @param index 
/// @return pointer to that value.
void* Cstl_Vec_get(Cstl_Vec const* self, usize index);


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


Cstl_Slice Cstl_Vec_as_slice(Cstl_Vec const* self);

void* Cstl_Vec_push_in_place(Cstl_Vec mut* self);

void* Cstl_Vec_extract(
    Cstl_Vec mut* self,
    Cstl_Ordering extremum_type,
    Cstl_Comparator cmp
);

void* Cstl_Vec_extract_min(
    Cstl_Vec mut* self, Cstl_Comparator cmp
);

void* Cstl_Vec_extract_max(
    Cstl_Vec mut* self, Cstl_Comparator cmp
);

Cstl_Slice Cstl_Vec_slice(Cstl_Vec mut* self, usize start, usize end);

Cstl_Slice Cstl_Vec_slice_unchecked(Cstl_Vec mut* self, usize start, usize end);



#ifdef USING_NAMESPACE_CSTL

    /// @brief Copies a value of type `Type` at index `index`.
    /// 
    /// @param Type a type of value.
    /// @param index the index of value.
    /// 
    /// @return The value at index `index`.
    #define Vec_get_value(self, Type, index) \
        Cstl_Vec_get_value(self, Type, index)


    #define Vec_from_elems(Type, elems...) \
        Cstl_Vec_from_elems(Type, elems)

    #define Vec_push_value(self, Type, value) \
        Cstl_Vec_push_value(self, Type, value)


    /// @brief Dynamic array implementation in C.
    typedef Cstl_Vec Vec;


    /// @brief Constructs an empty vector.
    /// 
    /// @param elem_size the size of an element of the vector.
    /// 
    /// @return New empty vector.
    Vec Vec_new(usize elem_size);


    /// @brief Constructs a vector with capacity `cap`.
    /// 
    /// @param cap start capacity of a vector.
    /// @param elem_size the size of an element of the vector
    /// 
    /// @return New empty vector.
    Vec Vec_with_capacity(usize cap, usize elem_size);


    /// @brief Constructs a vector from slice.
    /// 
    /// @param src a source slice to copy.
    /// 
    /// @return New vector with `src` elements.
    Vec Vec_from_slice(Slice src);


    /// @brief Clones the vector.
    /// 
    /// @return A vector clone.
    Vec Vec_clone(Vec const* self);


    /// @brief Clones the vector to existing one.
    /// 
    /// @param src the vector to be clonned.
    void Vec_clone_from(Vec* self, Vec const* src);


    /// @brief Frees a memory of the vector.
    void Vec_free(Vec const* self);


    /// @brief Pushes the value to the end of the vector.
    /// 
    /// @param value_ptr a pointer to the value.
    void Vec_push(Cstl_Vec* self, Addr value_ptr);


    /// @brief Decreases a length by one and returns a pointer to the poped value.
    /// 
    /// @return the poped value.
    void* Vec_pop(Vec* self);


    /// @brief Reserves exactly `additional_cap` as capacity.
    /// 
    /// @param additional_cap a capacity to reserve.
    void Vec_reserve_exact(Vec* self, usize additional_cap);


    /// @brief Reserves at least `additional_cap` as capacity.
    /// 
    /// @param additional_cap a minimal capacity to reserve.
    void Vec_reserve(Vec* self, usize additional_cap);


    /// @brief Clears the vector leaving capacity untouched.
    void Vec_clear(Vec* self);


    /// @brief Shrinks a capacity of the vector to `min_cap`.
    ///        Does nothing if `self->cap <= min_cap`.
    /// 
    /// @param min_cap the capatity to shrink to.
    void Vec_shrink_to(Vec* self, usize min_cap);


    /// @brief Shrinks a capacity of the vector to its length.
    void Vec_shrink_to_fit(Vec* self);


    /// @brief Computes address of a value at index `index`.
    /// 
    /// @param index 
    /// @return pointer to that value.
    void* Vec_get(Vec const* self, usize index);


    /// @brief Sets a value at index `index` to the value under `value_ptr`.
    /// 
    /// @param index value index.
    /// @param value_ptr a pointer to the value to be set.
    void Vec_set(Vec const* self, usize index, Addr value_ptr);


    /// @brief Fills the vector with a value.
    /// 
    /// @param value_ptr a pointer to that value. Should point outside of
    ///        vector or should be aligned with vector values.
    void Vec_fill(Vec const* self, Addr value_ptr);

    Slice Vec_as_slice(Vec const* self);

    void* Vec_push_in_place(Vec* self);

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_VEC_H_