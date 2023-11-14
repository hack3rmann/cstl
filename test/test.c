#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



i32 main(void) {
    AddrMut const ptr = mem_alloc(99999999999999999);

    assert_fmt(null_mut != ptr, "ptr = {Addr} is invalid pointer value", ptr);

    return EXIT_SUCCESS;
}