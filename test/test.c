#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



i32 main(void) {
    Vec mut nums = Vec_with_capacity(16, sizeof(i32));

    console_scan("{Vec:*, *:i32}", &mut nums);

    println("{Vec::i32}", &nums);

    Vec_free(&nums);

    return EXIT_SUCCESS;
}