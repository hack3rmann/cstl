#define USING_NAMESPACE_CSTL

#include <stdio.h>
#include <stdlib.h>

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"



void Slice_cut_greater(
    Slice mut* const self, Addr const value_ptr, Comparator const cmp
) {
    usize mut new_len = 0;

    for (usize mut i = 0; i < self->len; ++i) {
        if (Ordering_Greater != cmp(Slice_get(*self, i), value_ptr)) {
            Slice_set_unchecked(*self, new_len, Slice_get(*self, i));
            new_len += 1;
        }
    }

    self->len = new_len;
}



String f32_format_binexp(f32 value) {
    String mut number_fmt = String_with_capacity(64);

    Cstl_FloatImpl mut impl = Cstl_FloatImpl_from_f32(value);

    format_args(&mut number_fmt, "{char}1.", 0 < impl.sign ? '+' : '-');

    for (u64 mut cur_frac = 10 * (impl.frac >> 4)
        ; 0 != cur_frac
        ; cur_frac *= 10
    ) {
        u64 const digit = ((cur_frac >> 60) & 7) % 10;
        cur_frac &= 0x0FFFFFFFFFFFFFFF;

        println("frac = {u64:0b}, digit = {u64}", cur_frac, digit);

        String_push_ascii(&mut number_fmt, (char) digit + '0');
    }

    return number_fmt;
}



String f32_fmt_decimal(f32 const self) {
    String mut buf = String_with_capacity(64);

    todo("");

    return buf;
}



i32 main(void) {
    f32 const value = 2.1231552f;

    Cstl_FloatImpl const impl = Cstl_FloatImpl_from_f32(value);

    println("{*}", Cstl_FloatImpl_dbg, &impl);

    println(
        "{char}1.{u64} * 2 ^ {i32}",
        0 < impl.sign ? '+' : '-',
        impl.frac,
        impl.exp
    );

    String const binexp = f32_format_binexp(value);

    println("binexp = {String}", &binexp);

    String_free(&binexp);
}