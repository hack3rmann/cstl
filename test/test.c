#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



i32 main(void) {
    Vec_i32 mut vec = Vec_i32_with_capacity(256);
    vec.len = vec.cap;

    Cstl_mem_set((u8 mut*) vec.ptr, 1, sizeof(*vec.ptr) * vec.len);

    println("{Slice::i32:0b}", Vec_i32_as_slice(&vec));

    Vec_i32_free(&vec);

    return EXIT_SUCCESS;
}