#define USING_NAMESPACE_CSTL

#include <stdlib.h>
#include <stdio.h>

#include "../prelude.h"
#include "../format.h"
#include "../iterator.h"



i32 main(void) {
    Cstl_init();

    str const string = str("String example");
    Slice const slice = Slice_from_elems(i32, 1, 2, 3, 4, 5, 6);

    println("{str}! {Slice:(*, *):i32:0b}", string, slice);

    return EXIT_SUCCESS;
}