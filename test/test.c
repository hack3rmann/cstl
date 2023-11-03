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

    println("{str}! {Slice::i32}", string, slice);

    String const heap_string = String("Hello, World!");

    println("{String}", &heap_string);

    Cstl_String_free(&heap_string);

    return EXIT_SUCCESS;
}