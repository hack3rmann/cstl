#define USING_NAMESPACE_CSTL

#include <stdio.h>
#include <stdlib.h>

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"



void Slice_cut_greater(
    Slice mut* const self, Addr const value_ptr, Comparator const cmp
) {
    usize mut new_len = 0;

    for (usize mut i = 0; i < self->len; ++i) {
        if (Ordering_Greater != cmp(Slice_get(*self, i), value_ptr)) {
            Slice_set_unchecked(*self, new_len, Slice_get(*self, i));
            new_len += 1;
        }
    }

    self->len = new_len;
}



i32 main(void) {
    Slice mut nums = Slice(i32, 5, 4, 3, 2, 8, 1, 6);

    Slice_cut_greater(&mut nums, lit_ptr(i32, 3), i32_cmp);

    println("{Slice::i32}", nums);
}