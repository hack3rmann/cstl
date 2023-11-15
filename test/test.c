#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



i32 main(void) {
    isize const value = -10;

    BasicType const type = BasicType_from_value(value);

    println("{*}", BasicType_dbg, &type);

    return EXIT_SUCCESS;
}