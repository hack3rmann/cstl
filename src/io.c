#define USING_NAMESPACE_CSTL

#include "../include/cstl/io.h"
#include "../include/cstl/format.h"



extern int putchar(int symbol);



void Cstl_print(StrLit const fmt, ...) {
    VariadicArgs mut args;
    VariadicArgs_start(args, fmt);

    Cstl_print_impl(fmt, &mut args);

    VariadicArgs_end(args);
}

void Cstl_println(StrLit const fmt, ...) {
    VariadicArgs mut args;
    VariadicArgs_start(args, fmt);

    Cstl_print_impl(fmt, &mut args);
    putchar('\n');

    VariadicArgs_end(args);
}

void Cstl_print_impl(StrLit const fmt, VariadicArgs mut* const args) {
    Cstl_String mut out = Cstl_String_with_capacity(32);
    Cstl_format_args_impl(&mut out, fmt, args);

    Cstl_str_print(Cstl_String_as_str(&mut out));

    Cstl_String_free(&out);
}
