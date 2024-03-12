#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



i32 main(void) {
<<<<<<< HEAD
    AddrMut const ptr = mem_alloc(99999999999999999);

    assert_fmt(null_mut != ptr, "ptr = {Addr} is invalid pointer value", ptr);
=======
    Vec mut nums = Vec_with_capacity(16, sizeof(i32));

    console_scan("{Vec:*, *:i32}", &mut nums);

    println("{Vec::i32}", &nums);

    Vec_free(&nums);
>>>>>>> release/0.0.1a

    return EXIT_SUCCESS;
}