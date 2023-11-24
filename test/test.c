#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



i32 main(void) {
    u64 value;

    Cstl_CharStream mut stdin = Cstl_CharStream_STDIN;
    Cstl_CharStream_scan(&stdin, str("{u64}"), &mut value);

    println("value = {u64}", value);

    return EXIT_SUCCESS;
}