#define USING_NAMESPACE_CSTL

#include <stdio.h>
#include <stdlib.h>

#include "../prelude.h"
#include "../iterator.h"



i32 main(void) {
    Cstl_init();

    println("{Addr} = {Addr}", "Hello, World!", "Hello, World!");

    return EXIT_SUCCESS;
}