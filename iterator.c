#define USING_NAMESPACE_CSTL

#include "iterator.h"



Cstl_Range Cstl_Range_new(usize const start, usize const end) {
    return (Cstl_Range) {
        .start = start,
        .end = end
    };
}

usize Cstl_Range_next(Cstl_Range mut* const self) {
    if (self->end <= self->start) {
        return usize_MAX;
    }

    return self->start++;
}

bool Cstl_Range_is_expired(usize const* const ret) {
    return usize_MAX == *ret;
}