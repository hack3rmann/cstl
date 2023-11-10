#define USING_NAMESPACE_CSTL

#include <stdio.h>
#include <stdlib.h>

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"



i32 main(void) {
    Vec_i32 mut vec = Vec_i32_with_capacity(16);

    Vec_i32_push(&mut vec, 12);
    Vec_i32_push(&mut vec, 45);
    Vec_i32_push(&mut vec, 8765);

    println("{Slice::i32}", Vec_i32_as_slice(&vec));

    println("len = {usize}, cap = {usize}", vec.len, vec.cap);

    Vec_i32_free(&vec);
}