#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



i32 main(void) {
    usize const n_values = (usize) 1024 * 1024 * 1024 * 4;
    u16 mut* const values = Cstl_mem_alloc(n_values);

    Cstl_mem_set((u8 mut*) values, 42, n_values);

    Cstl_mem_free(values);

    return EXIT_SUCCESS;
}