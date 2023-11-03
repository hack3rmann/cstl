#include <stdio.h>
#include <stdlib.h>

#include "format.h"
#include "variadic.h"
#include "error.h"
#include "numeric.h"
#include "vec.h"



#define Cstl_impl_fmt_integer_Type_fn(Type) \
    void Cstl_ ## Type ## _fmt( \
        Cstl_String mut* buf, Cstl_str fmt, void const* value_ptr \
    ) { \
        i64 const value = (i64) *(Type*) value_ptr; \
        Cstl_i64_fmt(buf, fmt, &value); \
    }

Cstl_impl_fmt_integer_Type_fn(u8);
Cstl_impl_fmt_integer_Type_fn(u16);
Cstl_impl_fmt_integer_Type_fn(u32);
Cstl_impl_fmt_integer_Type_fn(u64);
Cstl_impl_fmt_integer_Type_fn(usize);
Cstl_impl_fmt_integer_Type_fn(i8);
Cstl_impl_fmt_integer_Type_fn(i16);
Cstl_impl_fmt_integer_Type_fn(i32);
Cstl_impl_fmt_integer_Type_fn(isize);



Cstl_BasicType Cstl_BasicType_from_str(Cstl_str const value) {
    if (Cstl_str_eq(value, Cstl_str_from_literal("u8"))) {
        return Cstl_BasicType_u8;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("i8"))) {
        return Cstl_BasicType_i8;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("u16"))) {
        return Cstl_BasicType_u16;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("i16"))) {
        return Cstl_BasicType_i16;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("u32"))) {
        return Cstl_BasicType_u32;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("i32"))) {
        return Cstl_BasicType_i32;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("u64"))) {
        return Cstl_BasicType_u64;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("i64"))) {
        return Cstl_BasicType_i64;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("f32"))) {
        return Cstl_BasicType_f32;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("f64"))) {
        return Cstl_BasicType_f64;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("usize"))) {
        return Cstl_BasicType_usize;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("isize"))) {
        return Cstl_BasicType_isize;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("Vec"))) {
        return Cstl_BasicType_Vec;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("Slice"))) {
        return Cstl_BasicType_Slice;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("String"))) {
        return Cstl_BasicType_String;
    }

    if (Cstl_str_eq(value, Cstl_str_from_literal("str"))) {
        return Cstl_BasicType_str;
    }

    // Cstl_todo("typed Vec and Slice format functions");

    Cstl_str_debug(value);
    Cstl_deny_fmt("invalid `type_name` = \'%s\'", (char const*) value.ptr);
}



void Cstl_i64_fmt(
    Cstl_String mut* const buf, Cstl_str mut fmt, Addr const value_ptr
) {
    u32 mut flags = 0;

    if (0 != fmt.len && 's' == *fmt.ptr) {
        flags |= Cstl_IntFmtFlags_Sign;

        fmt.ptr += 1;
        fmt.len -= 1;
    }

    if (0 != fmt.len && 'u' == *fmt.ptr) {
        flags |= Cstl_IntFmtFlags_Case;

        fmt.ptr += 1;
        fmt.len -= 1;
    }

    if (0 != fmt.len && 's' == *fmt.ptr) {
        flags |= Cstl_IntFmtFlags_Sign;

        fmt.ptr += 1;
        fmt.len -= 1;
    }

    u16 mut radix = 10;

    if (1 < fmt.len && '0' == *fmt.ptr) {
        fmt.ptr += 1;
        fmt.len -= 1;

        switch (*fmt.ptr) {
        case 'b': {
            radix = 2;
        } break;

        case 'o': {
            radix = 8;
        } break;

        case 'h': {
            radix = 16;
        } break;

        case 'x': {
            u16 const first_digit = fmt.ptr[1] - '0';
            u16 const second_digit = fmt.ptr[2] - '0';

            Cstl_assert(1 < first_digit && first_digit <= 9);

            radix = second_digit <= 9
                ? 10 * first_digit + second_digit
                : first_digit;
        } break;

        default:
            Cstl_deny_fmt("invalid radix letter `%c`", *(char const*) fmt.ptr);
        }
    }

    Cstl_i64_fmt_impl(buf, *(i64 const*) value_ptr, radix, flags);
}

void Cstl_i64_fmt_impl(
    Cstl_String mut* const buf,
    i64 const value,
    u16 const radix,
    u32 const flags
) {
    enum { RADIX_MAX = 10 + ('z' - 'a' + 1) };

    Bool const is_sign_shown = 0 != (flags & Cstl_IntFmtFlags_Sign);
    Bool const is_uppercase  = 0 != (flags & Cstl_IntFmtFlags_Case);

    Cstl_assert_fmt(
        1 < radix && radix <= RADIX_MAX,
        "integer format radix should be able to represent "
        "in alphanumerical symbols (radix = %u)",
        (u32) radix
    );

    char const first_alphabetic_symbol = is_uppercase ? 'A' : 'a';
    char const sign_symbol = 0 < value ? '+' : '-';

    if (value < 0 || (0 != value && is_sign_shown)) {
        Cstl_String_push_ascii(buf, sign_symbol);
    }

    if (0 == value) {
        Cstl_String_push_ascii(buf, '0');
        return;
    }

    u64 mut cur_value = i64_abs(value);
    usize mut number_len = 0;

    for (; 0 != cur_value; cur_value /= radix, ++number_len) {
        u16 const digit = cur_value % radix; 
        char const cur_symbol = digit < 10
            ? digit + '0'
            : digit - 10 + first_alphabetic_symbol;

        Cstl_String_push_ascii(buf, cur_symbol);
    }

    for (usize mut i = 0; i < number_len / 2; ++i) {
        // flip number
        Cstl_swap_8(
            buf->ptr + buf->len - number_len + i,
            buf->ptr + buf->len - i - 1
        );
    }
}

void Cstl_f32_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, void const* value_ptr
) {
    f64 const value = (f64) *(f32*) value_ptr;
    Cstl_f64_fmt(buf, fmt, &value);
}

void Cstl_f64_fmt(
    __attribute__((unused)) Cstl_String mut* const buf,
    __attribute__((unused)) Cstl_str const fmt,
    __attribute__((unused)) void const* value_ptr
) {
    // fmt = '(s)(u|l)(0b|0o|0h|0xP)((+|-)(.|,)(N))'

}

void Cstl_Vec_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, void const* value_ptr
) {
    Cstl_Slice_fmt(buf, fmt, value_ptr);
}

void Cstl_Slice_fmt(
    Cstl_String mut* const buf, Cstl_str mut fmt, void const* value_ptr
) {
    // fmt = '(*,*):Type:type_args'

    Cstl_Slice const slice = *(Cstl_Slice const*) value_ptr;

    Cstl_str const colon_delim = Cstl_str_from_literal(":");
    Cstl_str const star_delim = Cstl_str_from_literal("*");

    Cstl_str mut brace_fmt = Cstl_str_split_one(&mut fmt, colon_delim);
    Cstl_str mut l_brace, mut delim, mut r_brace;

    Cstl_str const type_name = Cstl_str_split_one(&mut fmt, colon_delim);
    Cstl_BasicType const type = Cstl_BasicType_from_str(type_name);

    if (0 != brace_fmt.len) {
        l_brace = Cstl_str_split_one(&mut brace_fmt, star_delim);
        delim = Cstl_str_split_one(&mut brace_fmt, star_delim);
        r_brace = Cstl_str_split_one(&mut brace_fmt, star_delim);
    } else {
        l_brace = Cstl_str_from_literal("[");
        delim = Cstl_str_from_literal(", ");
        r_brace = Cstl_str_from_literal("]");
    }

    Cstl_String_append(buf, l_brace);

#   define switch_case_numeric(Type) \
        case Cstl_BasicType_ ## Type: { \
            for (usize mut i = 0; i < slice.len; ++i) { \
                if (0 != i) { \
                    Cstl_String_append(buf, delim); \
                } \
                Type const value = Cstl_Slice_get_value(&slice, Type, i); \
                Cstl_ ## Type ## _fmt(buf, fmt, &value); \
            } \
        } break

#   define switch_case_non_numeric(Type) \
        case Cstl_BasicType_ ## Type: { \
            for (usize mut i = 0; i < slice.len; ++i) { \
                if (0 != i) { \
                    Cstl_String_append(buf, delim); \
                } \
                Cstl_ ## Type const value \
                    = Cstl_Slice_get_value(&slice, Cstl_ ## Type, i); \
                Cstl_ ## Type ## _fmt(buf, fmt, &value); \
            } \
        } break

    switch (type) {
    switch_case_numeric(i8);
    switch_case_numeric(u8);
    switch_case_numeric(i16);
    switch_case_numeric(u16);
    switch_case_numeric(i32);
    switch_case_numeric(u32);
    switch_case_numeric(i64);
    switch_case_numeric(u64);
    switch_case_numeric(isize);
    switch_case_numeric(usize);
    switch_case_numeric(f32);
    switch_case_numeric(f64);
    switch_case_non_numeric(Slice);
    switch_case_non_numeric(Vec);
    switch_case_non_numeric(str);
    switch_case_non_numeric(String);

    case Cstl_BasicType_Invalid:
        // fallthrough
    default:
        Cstl_deny_fmt("invalid enum Cstl_BasicType value %u", (u32) type);
    }

    Cstl_String_append(buf, r_brace);

#   undef switch_case_numeric
#   undef switch_case_non_numeric
}

    void Cstl_String_fmt(
        Cstl_String mut* const buf, Cstl_str const fmt, void const* value_ptr
    ) {
        Cstl_str_fmt(buf, fmt, value_ptr);
    }

    void Cstl_str_fmt(
        Cstl_String mut* const buf, Cstl_str const fmt, void const* value_ptr
    ) {
        Cstl_assert_fmt(
            0 == fmt.len,
            "str format string should be empty, but the value is %zu, fmt = '%s'",
            fmt.len,
            (char const*) fmt.ptr
        );

        Cstl_String_append(buf, *(Cstl_str const*) value_ptr);
    }

    Cstl_FormatFn Cstl_FormatFn_from_type_name(Cstl_str const type_name) {
        return Cstl_FormatFn_from_basic_type(Cstl_BasicType_from_str(type_name));
    }

    Cstl_FormatFn Cstl_FormatFn_from_basic_type(Cstl_BasicType const type) {
        switch (type) {
        case Cstl_BasicType_u8:     return Cstl_u8_fmt;
        case Cstl_BasicType_i8:     return Cstl_i8_fmt;
        case Cstl_BasicType_u16:    return Cstl_u16_fmt;
        case Cstl_BasicType_i16:    return Cstl_i64_fmt;
        case Cstl_BasicType_u32:    return Cstl_u32_fmt;
        case Cstl_BasicType_i32:    return Cstl_i32_fmt;
        case Cstl_BasicType_u64:    return Cstl_u64_fmt;
        case Cstl_BasicType_i64:    return Cstl_i64_fmt;
        case Cstl_BasicType_f32:    return Cstl_f32_fmt;
        case Cstl_BasicType_f64:    return Cstl_f64_fmt;
        case Cstl_BasicType_usize:  return Cstl_usize_fmt;
        case Cstl_BasicType_isize:  return Cstl_isize_fmt;
        case Cstl_BasicType_Vec:    return Cstl_Vec_fmt;
        case Cstl_BasicType_Slice:  return Cstl_Slice_fmt;
        case Cstl_BasicType_String: return Cstl_String_fmt;
        case Cstl_BasicType_str:    return Cstl_str_fmt;

        case Cstl_BasicType_Invalid:
            // fallthrough
        default:
            Cstl_deny_fmt("invalid enum Cstl_BasicType value %u", (u32) type);
        }
    }

    void Cstl__internal_format_scope_impl(
        Cstl_String mut* const buf,
        Cstl_str const fmt,
        VariadicArgs mut* args
    ) {
        Cstl_str mut type_name = Cstl_str_DEFAULT;
        Cstl_str mut fmt_args = Cstl_str_DEFAULT;

        usize mut i = 1;

        if ('*' == fmt.ptr[1]) {
            type_name.ptr = fmt.ptr + 1;
            type_name.len = 1;
            i += 1;
        } else {
            type_name.ptr = fmt.ptr + 1;

            for (; i < fmt.len && (
                Cstl_char_is_ascii_letter(fmt.ptr[i])
                || Cstl_char_is_numeric_letter(fmt.ptr[i])
            ); ++i);

            type_name.len = i - 1;
        }

        if (':' == fmt.ptr[i]) {
            fmt_args.ptr = fmt.ptr + i + 1;
            fmt_args.len = fmt.len - i - 2;
        }

        Cstl_BasicType const type = Cstl_BasicType_from_str(type_name);

        Cstl_FormatFn const format_fn
            = Cstl_str_eq(type_name, Cstl_str_from_literal("*"))
            ? VariadicArgs_get(*args, Cstl_FormatFn)
            : Cstl_FormatFn_from_basic_type(type);

        switch (type) {
        case Cstl_BasicType_u8: {
            u8 const value = VariadicArgs_get(*args, unsigned);
            format_fn(buf, fmt_args, &value);
        } break;

        case Cstl_BasicType_i8: {
            i8 const value = VariadicArgs_get(*args, int);
            format_fn(buf, fmt_args, &value);
        } break;

        case Cstl_BasicType_u16: {
            u16 const value = VariadicArgs_get(*args, unsigned);
            format_fn(buf, fmt_args, &value);
        } break;

        case Cstl_BasicType_i16: {
            i16 const value = VariadicArgs_get(*args, int);
            format_fn(buf, fmt_args, &value);
        } break;

        case Cstl_BasicType_u32: {
            u32 const value = VariadicArgs_get(*args, u32);
            format_fn(buf, fmt_args, &value);
        } break;

        case Cstl_BasicType_i32: {
            i32 const value = VariadicArgs_get(*args, i32);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_u64: {
            u64 const value = VariadicArgs_get(*args, u64);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_i64: {
            i64 const value = VariadicArgs_get(*args, i64);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_f32: {
            f32 const value = VariadicArgs_get(*args, f64);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_f64: {
            f64 const value = VariadicArgs_get(*args, f64);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_usize: {
            usize const value = VariadicArgs_get(*args, usize);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_isize: {
            isize const value = VariadicArgs_get(*args, isize);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_Vec: {
            Cstl_Vec const* const value_ptr
                = VariadicArgs_get(*args, Cstl_Vec const*);
            format_fn(buf, fmt_args, value_ptr);
        } break;
        
        case Cstl_BasicType_Slice: {
            Cstl_Slice const value = VariadicArgs_get(*args, Cstl_Slice);
            format_fn(buf, fmt_args, &value);
        } break;
        
        case Cstl_BasicType_String: {
            Cstl_String const* const value_ptr
                = VariadicArgs_get(*args, Cstl_String const*);
            format_fn(buf, fmt_args, value_ptr);
        } break;
        
        case Cstl_BasicType_str: {
            Cstl_str const value = VariadicArgs_get(*args, Cstl_str);
            format_fn(buf, fmt_args, &value);
        } break;

        case Cstl_BasicType_Invalid:
            // fallthrough
        default:
            break;
        }
    }

    void Cstl_format_args_impl(
        Cstl_String mut* buf, StrLit fmt, VariadicArgs mut* args
    ) {
        for (; '\0' != *fmt;) {
            switch (fmt[0]) {
            case '{': {
                Cstl_assert('\0' != fmt[1]);

                if ('{' == fmt[1]) {
                    Cstl_String_push_ascii(buf, '{');
                    fmt += 1;
                } else {
                    char const* scope_end_ptr = fmt;

                    for (; '}' != *scope_end_ptr; ++scope_end_ptr);

                    Cstl_str const unary_fmt = {
                        .ptr = (u8*) fmt,
                        .len = scope_end_ptr - fmt + 1
                    };

                    Cstl__internal_format_scope_impl(buf, unary_fmt, args);

                    fmt += unary_fmt.len;
                }
            } break;

            case '}': {
                Cstl_assert_msg(
                    '}' == fmt[1],
                    "ensure escape \'}\' with another \'}\': \'}}\'"
                );

                Cstl_String_push_ascii(buf, '}');
                fmt += 1;
            } break;

            default: {
                u8 const leader_byte = *(u8*) fmt;

                Cstl_Utf8ByteClassification const class
                    = Cstl_Utf8ByteClassification_of(leader_byte);

                usize const n_bytes = class + 1;

                switch (class) {
                case Cstl_Utf8ByteClassification_SingleByte:
                case Cstl_Utf8ByteClassification_PairByteEntry:
                case Cstl_Utf8ByteClassification_TripleByteEntry:
                    // fallthrough
                case Cstl_Utf8ByteClassification_QuadByteEntry: {
                    // empty
                } break;

                case Cstl_Utf8ByteClassification_TailByte:
                case Cstl_Utf8ByteClassification_InvalidByte:
                    // fallthrough
                default: {
                    Cstl_unreachable_msg("invalid string");
                } break;
                }

                Cstl_String_append_bytes(
                    buf,
                    (Cstl_Slice_u8) { .ptr = (u8*) fmt, .len = n_bytes }
                );

                fmt += n_bytes;
            } break;
            }
        }
    }

    void Cstl_format_args(Cstl_String mut* const buf, StrLit mut fmt, ...) {
        VariadicArgs mut args;
        VariadicArgs_start(args, fmt);

        Cstl_format_args_impl(buf, fmt, &mut args);

        VariadicArgs_end(args);
    }

    Cstl_String Cstl_format(StrLit const fmt, ...) {
        VariadicArgs mut args;
        VariadicArgs_start(args, fmt);

        Cstl_String mut buf = Cstl_String_DEFAULT;

        Cstl_format_args_impl(&mut buf, fmt, &mut args);

        VariadicArgs_end(args);

        return buf;
    }