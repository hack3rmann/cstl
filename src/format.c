#define USING_NAMESPACE_CSTL

#include "../include/cstl/format.h"
#include "../include/cstl/variadic.h"
#include "../include/cstl/error.h"
#include "../include/cstl/numeric.h"
#include "../include/cstl/vec.h"



extern int getchar(void);



#define Cstl_impl_fmt_integer_Type_fn(Type) \
    void Cstl_ ## Type ## _fmt( \
        Cstl_String mut* buf, Cstl_str fmt, Addr value_ptr \
    ) { \
        i64 const value = (i64) *(Type*) value_ptr; \
        Cstl_i64_fmt(buf, fmt, &value); \
    }

Cstl_impl_fmt_integer_Type_fn(u8);
Cstl_impl_fmt_integer_Type_fn(u16);
Cstl_impl_fmt_integer_Type_fn(u32);
Cstl_impl_fmt_integer_Type_fn(usize);
Cstl_impl_fmt_integer_Type_fn(i8);
Cstl_impl_fmt_integer_Type_fn(i16);
Cstl_impl_fmt_integer_Type_fn(i32);
Cstl_impl_fmt_integer_Type_fn(isize);



Cstl_FloatImpl Cstl_FloatImpl_from_f32(f32 const value) {
    union {
        f32 value;
        u32 bits;
    } mut x = { .value = value };

    x.value = value;

    return (Cstl_FloatImpl) {
        .sign = 1 - 2 * (i32) (x.bits >> 31),
        .exp = ((x.bits >> 23) & 255) - 127,
        .frac = (u64) (x.bits & 8388607) << 41
    };
}

Cstl_FloatImpl Cstl_FloatImpl_from_f64(f64 const value) {
    union {
        f64 value;
        u64 bits;
    } mut x = { .value = value };

    return (Cstl_FloatImpl) {
        .sign = 1 - 2 * (i32) (x.bits >> 63),
        .exp = ((x.bits >> 52) & 2048) - 1023,
        .frac = (u64) (x.bits & 4503599627370495) << 12
    };
}

f32 Cstl_FloatImpl_to_f32(Cstl_FloatImpl const self) {
    union {
        f32 value;
        u32 bits;
    } mut x = { .bits = 0 };

    x.bits |= ((1 - self.sign) / 2) << 31;
    x.bits |= (self.exp + 127) << 23;
    x.bits |= (self.frac >> 41) & 8388607;

    return x.value;
}

f64 Cstl_FloatImpl_to_f64(Cstl_FloatImpl const self) {
    union {
        f64 value;
        u64 bits;
    } mut x = { .bits = 0 };

    x.bits |= (u64) ((1 - self.sign) / 2) << 63;
    x.bits |= (u64) (self.exp + 1023) << 52;
    x.bits |= (u64) (self.frac >> 12) & 4503599627370495;

    return x.value;
}

void Cstl_FloatImpl_dbg(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    Cstl_assert(0 == fmt.len);

    Cstl_FloatImpl const* const self = (Cstl_FloatImpl const*) value_ptr;

    Cstl_format_args(
        buf,
        "FloatImpl {{ sign: {i32}, exp: {i32:0b}, frac: {u64:0b} }}",
        self->sign, self->exp, self->frac
    );
}

Cstl_FloatFormatDescriptor Cstl_FloatFormatDescriptor_parse(
    Cstl_str mut fmt
) {
    // fmt = '(s)(u)(0b|0o|0h|0xP)((+|-)(.|,)(N))'

    Cstl_FloatFormatDescriptor mut desc = Cstl_FloatFormatDescriptor_DEFAULT;

    bool mut is_end = false;

    while (!is_end) {
        usize mut step_size = 1;
        
        switch (*fmt.ptr) {
        case '+': case '_': case '.': case ',':
            is_end = true;
            break;
        case 's':
            desc.flags |= Cstl_IntegerFmtFlags_ShowSign;
            break;
        case 'u':
            desc.flags |= Cstl_IntegerFmtFlags_Uppercase;
            break;
        case 'e':
            desc.notation = Cstl_FloatNotation_Scientific;
            break;

        case '0': {
            step_size += 1;

            switch (fmt.ptr[1]) {
            case 'b':
                desc.radix = 2;
                break;
            case 'o':
                desc.radix = 8;
                break;
            case 'h':
                desc.radix = 16;
                break;

            case 'x': {
                u16 const first_digit = fmt.ptr[2] - '0';
                u16 const second_digit = fmt.ptr[3] - '0';

                Cstl_assert(0 < first_digit && first_digit <= 9);
                Cstl_assert(bool_implies(first_digit < 2, second_digit <= 9));

                desc.radix = second_digit <= 9
                    ? first_digit * 10 + second_digit
                    : first_digit;

                step_size += second_digit <= 9 ? 2 : 1;
            } break;

            default:
                Cstl_deny("invalid format");
            }
        } break;

        default:
            Cstl_deny("invalid format");
        }

        fmt.ptr += step_size;
        fmt.len -= step_size;
    }

    switch (fmt.ptr[0]) {
    case '-': {
        desc.round = Cstl_FloatRound_Down;
        fmt.ptr += 1;
    } break;

    case '+': {
        desc.round = Cstl_FloatRound_Up;
        fmt.ptr += 1;
    } break;

    default:
        break;
    }

    switch (fmt.ptr[0]) {
    case '.':
        // fallthrough
    case ',': {
        desc.fraction_delim = fmt.ptr[0];

        u16 const first_digit = fmt.ptr[1] - '0';
        u16 const second_digit = fmt.ptr[2] - '0';

        Cstl_assert(first_digit <= 9);
        
        desc.n_fraction_digits = second_digit <= 9
            ? first_digit * 10 + second_digit
            : first_digit;
    } break;

    default:
        break;
    }

    return desc;
}



void Cstl_Addr_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    bool const is_uppercase = 0 < fmt.len && 'u' == *(char const*) fmt.ptr;

    Cstl_String_append(buf, str("0x"));
    Cstl_usize_fmt(buf, is_uppercase ? str("u0h") : str("0h"), value_ptr);
}

void Cstl_AddrMut_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    Cstl_Addr_fmt(buf, fmt, value_ptr);
}

void Cstl_char_fmt(
    Cstl_String mut* const buf, Cstl_str mut fmt, Addr const value_ptr
) {
    Cstl_assert(0 == fmt.len);

    Cstl_String_push_ascii(buf, *(char const*) value_ptr);
}

void Cstl_u64_fmt(
    Cstl_String mut* const buf, Cstl_str mut fmt, Addr const value_ptr
) {
    Cstl_IntegerFormatDescriptor mut desc
        = Cstl_IntegerFormatDescriptor_parse(fmt);

    desc.value = *(u64 const*) value_ptr;

    Cstl_x64_fmt_impl(buf, desc);
}

void Cstl_i64_fmt(
    Cstl_String mut* const buf, Cstl_str mut fmt, Addr const value_ptr
) {
    Cstl_IntegerFormatDescriptor mut desc
        = Cstl_IntegerFormatDescriptor_parse(fmt);

    i64 const value = *(i64 const*) value_ptr;

    desc.value = (u64) i64_abs(value);
    desc.sign = 1 - 2 * (value < 0);

    Cstl_x64_fmt_impl(buf, desc);
}

Cstl_IntegerFormatDescriptor Cstl_IntegerFormatDescriptor_parse(
    Cstl_str mut value
) {
    u32 mut flags = 0;

    if (0 != value.len && 's' == *value.ptr) {
        flags |= Cstl_IntegerFmtFlags_ShowSign;

        value.ptr += 1;
        value.len -= 1;
    }

    if (0 != value.len && 'u' == *value.ptr) {
        flags |= Cstl_IntegerFmtFlags_Uppercase;

        value.ptr += 1;
        value.len -= 1;
    }

    if (0 != value.len && 's' == *value.ptr) {
        flags |= Cstl_IntegerFmtFlags_ShowSign;

        value.ptr += 1;
        value.len -= 1;
    }

    u16 mut radix = 10;

    if (1 < value.len && '0' == *value.ptr) {
        value.ptr += 1;
        value.len -= 1;

        switch (*value.ptr) {
        case 'b': { radix = 2;  break; }
        case 'o': { radix = 8;  break; }
        case 'h': { radix = 16; break; }

        case 'x': {
            u16 const first_digit = value.ptr[1] - '0';
            u16 const second_digit = value.ptr[2] - '0';

            Cstl_assert_fmt(
                1 <= first_digit && first_digit <= 9,
                "`first_digit` = {u16}",
                first_digit
            );

            Cstl_assert(bool_implies(9 < second_digit, 1 < first_digit));

            radix = second_digit <= 9
                ? 10 * first_digit + second_digit
                : first_digit;
        } break;

        default:
            Cstl_deny_fmt(
                "invalid radix letter {char}", *(char const*) value.ptr
            );
        }
    }

    return (Cstl_IntegerFormatDescriptor) {
        .value = 0,
        .flags = flags,
        .radix = radix,
        .sign = 1
    };
}

void Cstl_x64_fmt_impl(
    Cstl_String mut* const buf, Cstl_IntegerFormatDescriptor const desc
) {
    enum { RADIX_MAX = 10 + ('z' - 'a' + 1) };

    bool const is_sign_shown = 0 != (desc.flags & Cstl_IntegerFmtFlags_ShowSign);
    bool const is_uppercase  = 0 != (desc.flags & Cstl_IntegerFmtFlags_Uppercase);

    Cstl_assert_fmt(
        1 < desc.radix && desc.radix <= RADIX_MAX,
        "integer format radix should be able to represent \
        in alphanumerical symbols (radix = {u16})",
        desc.radix
    );

    char const first_alphabetic_symbol = is_uppercase ? 'A' : 'a';
    char const sign_symbol = 0 < desc.sign ? '+' : '-';

    if (desc.sign < 0 || (0 != desc.sign && is_sign_shown)) {
        Cstl_String_push_ascii(buf, sign_symbol);
    }

    if (0 == desc.value) {
        Cstl_String_push_ascii(buf, '0');
        return;
    }

    u64 mut cur_value = i64_abs(desc.value);
    usize mut number_len = 0;

    for (; 0 != cur_value; cur_value /= desc.radix, ++number_len) {
        u16 const digit = cur_value % desc.radix;
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
    Cstl_String mut* const buf, Cstl_str const fmt, Addr value_ptr
) {
    f64 const value = (f64) *(f32*) value_ptr;
    Cstl_f64_fmt(buf, fmt, &value);
}

bool Cstl__internal_f64_fmt_is_end(char const value) {
    return '+' == value || '-' == value || '.' == value || ',' == value;
}

void Cstl_f64_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    Cstl_FloatFormatDescriptor mut desc = Cstl_FloatFormatDescriptor_parse(fmt);

    desc.value = *(f64 const*) value_ptr;

    Cstl_f64_fmt_impl(buf, desc);
}

void Cstl_f64_fmt_impl(
    Cstl_String mut* const buf, Cstl_FloatFormatDescriptor const desc
) {
    // FIXME: remove `UNUSED`
    UNUSED bool const is_fraction_fixed = u16_MAX != desc.n_fraction_digits;

    Cstl_FloatImpl const impl = Cstl_FloatImpl_from_f64(desc.value);

    if (0 != (desc.flags & Cstl_IntegerFmtFlags_ShowSign)) {
        Cstl_String_push_ascii(buf, 0 < impl.sign ? '+' : '-');
    } else if (impl.sign < 0) {
        Cstl_String_push_ascii(buf, '-');
    }

    switch (desc.notation) {
    case Cstl_FloatNotation_Scientific: {
        Cstl_String_append(buf, str("1."));

        for (u64 mut cur_frac = impl.frac; 0 != cur_frac; cur_frac *= 10) {
            
        }
    } break;

    case Cstl_FloatNotation_Point:
        Cstl_todo("point notation");

    default:
        Cstl_deny_fmt(
            "invalid enum FloatNotation value `{u32}`", (u32) desc.notation
        );
    }

    Cstl_todo("");
}

void Cstl_bool_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    bool const value = *(bool const*) value_ptr;

    Cstl_String_append(buf, value ? str("true") : str("false"));
}

void Cstl_Vec_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr value_ptr
) {
    Cstl_Slice_fmt(buf, fmt, value_ptr);
}

void Cstl_Slice_fmt(
    Cstl_String mut* const buf, Cstl_str mut fmt, Addr value_ptr
) {
    // fmt = '(*,*):Type:type_args'

    Cstl_Slice const slice = *(Cstl_Slice const*) value_ptr;

    Cstl_str const colon_delim = str(":");
    Cstl_str const star_delim = str("*");

    Cstl_str mut brace_fmt = Cstl_str_split_once(&mut fmt, colon_delim);
    Cstl_str mut l_brace, mut delim, mut r_brace;

    Cstl_str const type_name = Cstl_str_split_once(&mut fmt, colon_delim);
    Cstl_FormattableType const type = Cstl_FormattableType_parse(type_name);

    if (0 != brace_fmt.len) {
        l_brace = Cstl_str_split_once(&mut brace_fmt, star_delim);
        delim = Cstl_str_split_once(&mut brace_fmt, star_delim);
        r_brace = Cstl_str_split_once(&mut brace_fmt, star_delim);
    } else {
        l_brace = str("[");
        delim = str(", ");
        r_brace = str("]");
    }

    Cstl_String_append(buf, l_brace);

#   define switch_case(Type) \
        case Cstl_BasicType_ ## Type: { \
            for (usize mut i = 0; i < slice.len; ++i) { \
                if (0 != i) { \
                    Cstl_String_append(buf, delim); \
                } \
                Type const value = Cstl_Slice_get_value(slice, Type, i); \
                Cstl_ ## Type ## _fmt(buf, fmt, &value); \
            } \
        } break

    // TODO: allow generic formats
    Cstl_assert(Cstl_FormattableType_Case_Type == type.descriptor);

    switch (type.type) {
    switch_case(i8);
    switch_case(u8);
    switch_case(i16);
    switch_case(u16);
    switch_case(i32);
    switch_case(u32);
    switch_case(i64);
    switch_case(u64);
    switch_case(isize);
    switch_case(usize);
    switch_case(Addr);
    switch_case(f32);
    switch_case(f64);
    switch_case(bool);
    switch_case(CStr);
    switch_case(CStrMut);
    switch_case(StrLit);
    switch_case(char);
    switch_case(Slice);
    switch_case(Vec);
    switch_case(String);
    switch_case(str);

    case Cstl_BasicType_Invalid:
        // fallthrough
    default:
        Cstl_deny_fmt("invalid enum FormattableType value {u32}", (u32) type.type);
    }

    Cstl_String_append(buf, r_brace);

#   undef switch_case_numeric
#   undef switch_case_non_numeric
}

void Cstl_String_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr value_ptr
) {
    Cstl_str_fmt(buf, fmt, value_ptr);
}

void Cstl_str_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr value_ptr
) {
    Cstl_assert_fmt(
        0 == fmt.len,
        "str format string should be empty, but the value is '{str}'"
        " with length `{usize}`",
        fmt, fmt.len
    );

    Cstl_String_append(buf, *(Cstl_str const*) value_ptr);
}

void Cstl_CStr_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    CStr const value = *(CStr const*) value_ptr;
    usize const len = CStr_len(value);

    Cstl_str const string = Cstl_str_from_utf8_unchecked((u8 mut*) value, len);

    Cstl_str_fmt(buf, fmt, &string);
}

void Cstl_CStrMut_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    Cstl_CStr_fmt(buf, fmt, value_ptr);
}

void Cstl_StrLit_fmt(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    Cstl_CStr_fmt(buf, fmt, value_ptr);
}

Cstl_FormatFn Cstl_FormatFn_from_type_name(Cstl_str const type_name) {
    return Cstl_FormatFn_from_formattable_type(
        Cstl_FormattableType_parse(type_name)
    );
}

Cstl_FormattableType Cstl_FormattableType_parse(Cstl_str const value) {
    if (str_eq(value, str("*"))) {
        return (Cstl_FormattableType) {
            .descriptor = Cstl_FormattableType_Case_Generic,
            .type = Cstl_BasicType_DEFAULT
        };
    } else {
        return (Cstl_FormattableType) {
            .descriptor = Cstl_FormattableType_Case_Type,
            .type = Cstl_BasicType_parse(value)
        };
    }
}

Cstl_FormatFn Cstl_FormatFn_from_formattable_type(
    Cstl_FormattableType const type
) {
    switch (type.type) {
    case Cstl_BasicType_u8:        return Cstl_u8_fmt;
    case Cstl_BasicType_i8:        return Cstl_i8_fmt;
    case Cstl_BasicType_u16:       return Cstl_u16_fmt;
    case Cstl_BasicType_i16:       return Cstl_i64_fmt;
    case Cstl_BasicType_u32:       return Cstl_u32_fmt;
    case Cstl_BasicType_i32:       return Cstl_i32_fmt;
    case Cstl_BasicType_u64:       return Cstl_u64_fmt;
    case Cstl_BasicType_i64:       return Cstl_i64_fmt;
    case Cstl_BasicType_f32:       return Cstl_f32_fmt;
    case Cstl_BasicType_f64:       return Cstl_f64_fmt;
    case Cstl_BasicType_bool:      return Cstl_bool_fmt;
    case Cstl_BasicType_usize:     return Cstl_usize_fmt;
    case Cstl_BasicType_isize:     return Cstl_isize_fmt;
    case Cstl_BasicType_char:      return Cstl_char_fmt;
    case Cstl_BasicType_Addr:      return Cstl_Addr_fmt;
    case Cstl_BasicType_AddrMut:   return Cstl_AddrMut_fmt;
    case Cstl_BasicType_Vec:       return Cstl_Vec_fmt;
    case Cstl_BasicType_Slice:     return Cstl_Slice_fmt;
    case Cstl_BasicType_String:    return Cstl_String_fmt;
    case Cstl_BasicType_str:       return Cstl_str_fmt;
    case Cstl_BasicType_CStr:      return Cstl_CStr_fmt;
    case Cstl_BasicType_CStrMut:   return Cstl_CStrMut_fmt;
    case Cstl_BasicType_StrLit:    return Cstl_StrLit_fmt;

    case Cstl_BasicType_Invalid:
        // fallthrough
    default:
        Cstl_deny_fmt(
            "invalid enum Cstl_BasicType value {u32}", (u32) type.type
        );
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

    Cstl_FormattableType const type = Cstl_FormattableType_parse(type_name);

    Cstl_FormatFn const format_fn
        = Cstl_FormattableType_Case_Generic == type.descriptor
        ? VariadicArgs_get(*args, Cstl_FormatFn)
        : Cstl_FormatFn_from_formattable_type(type);

    if (Cstl_FormattableType_Case_Generic == type.descriptor) {
        Addr const value_ptr = VariadicArgs_get(*args, Addr);
        format_fn(buf, fmt_args, value_ptr);

        return;
    }

    switch (type.type) {
    case Cstl_BasicType_u8: {
        u8 const value = VariadicArgs_get(*args, uint);
        format_fn(buf, fmt_args, &value);
    } break;

    case Cstl_BasicType_i8: {
        i8 const value = VariadicArgs_get(*args, int);
        format_fn(buf, fmt_args, &value);
    } break;

    case Cstl_BasicType_u16: {
        u16 const value = VariadicArgs_get(*args, uint);
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

    case Cstl_BasicType_AddrMut:
        // fallthrough
    case Cstl_BasicType_Addr: {
        Addr const value = VariadicArgs_get(*args, Addr);
        format_fn(buf, fmt_args, &value);
    } break;

    case Cstl_BasicType_char: {
        char const value = VariadicArgs_get(*args, int);
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

    case Cstl_BasicType_bool: {
        bool const value = (bool) VariadicArgs_get(*args, uint);
        format_fn(buf, fmt_args, &value);
    } break;

    case Cstl_BasicType_StrLit:
    case Cstl_BasicType_CStr:
        // fallthrough
    case Cstl_BasicType_CStrMut: {
        CStr const value = VariadicArgs_get(*args, CStr);
        format_fn(buf, fmt_args, &value);
    } break;

    case Cstl_BasicType_Invalid:
        // fallthrough
    default:
        break;
    }
}

void Cstl_format_args_impl(
    Cstl_String mut* const buf, StrLit mut fmt, VariadicArgs mut* const args
) {
    for (; '\0' != *fmt;) {
        switch (fmt[0]) {
        case '{': {
            Cstl_assert('\0' != fmt[1]);

            if ('{' == fmt[1]) {
                Cstl_String_push_ascii(buf, '{');
                fmt += 2;
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
                "ensure escape '}' with another '}': '}}'"
            );

            Cstl_String_push_ascii(buf, '}');
            fmt += 2;
        } break;

        default: {
            u8 const leader_byte = *(u8 const*) fmt;

            Cstl_Utf8ByteType const class
                = Cstl_Utf8ByteType_of(leader_byte);

            usize const n_bytes = class + 1;

            switch (class) {
            case Cstl_Utf8ByteType_SingleByte:
            case Cstl_Utf8ByteType_PairByteEntry:
            case Cstl_Utf8ByteType_TripleByteEntry:
                // fallthrough
            case Cstl_Utf8ByteType_QuadByteEntry: {
                // empty
            } break;

            case Cstl_Utf8ByteType_TailByte:
            case Cstl_Utf8ByteType_InvalidByte:
                // fallthrough
            default: {
                Cstl_unreachable_msg("invalid string");
            } break;
            }

            Cstl_String_append_bytes(
                buf,
                (Cstl_Slice_u8) { .ptr = (u8 mut*) fmt, .len = n_bytes }
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

u32 Cstl_GetCharFn_stdin_get(UNUSED AddrMut const _) {
    return (u32) getchar();
}

u32 Cstl_GetCharFn_str_get(AddrMut const value_addr) {
    str mut* const value = (str mut*) value_addr;

    if (0 == value->len) {
        return u32_MAX;
    }

    u32 const result = (u32) value->ptr[0];

    value->ptr += 1;
    value->len -= 1;

    return result;
}



bool Cstl_IsCharStreamExpiredFn_stdin_is_expired(UNUSED u32 const _) {
    return false;
}

bool Cstl_IsCharStreamExpiredFn_str_is_expired(u32 const ret) {
    return u32_MAX == ret;
}



bool Cstl_IsSkippedFn_skip_spaces(char const symbol) {
    return Cstl_char_is_whitespace(symbol);
}

bool Cstl_IsSkippedFn_skip_nothing(UNUSED char _) {
    return false;
}



bool Cstl_IsDelimFn_is_new_line(char const value) {
    return '\n' == value || '\r' == value;
}



Cstl_CharStream const Cstl_CharStream_STDIN = {
    .src_ptr = null_mut,
    .get = Cstl_GetCharFn_STDIN,
    .is_expired = Cstl_IsCharStreamExpiredFn_STDIN,
    .is_skipped = Cstl_IsSkippedFn_DEFAULT,
    .is_delim = Cstl_IsDelimFn_DEFAULT
};

Cstl_CharStream Cstl_CharStream_from_str(Cstl_str mut* const value_ptr) {
    return (Cstl_CharStream) {
        .src_ptr = value_ptr,
        .is_expired = Cstl_IsCharStreamExpiredFn_STR,
        .get = Cstl_GetCharFn_str_get,
        .is_skipped = Cstl_IsSkippedFn_DEFAULT,
        .is_delim = Cstl_IsDelimFn_DEFAULT
    };
}

u32 Cstl_CharStream_next(AddrMut const self_addr) {
    Cstl_CharStream mut* const self = self_addr;

    char mut result;

    do {
        result = self->get(self->src_ptr);
    } while (self->is_skipped(result));

    return result;
}

void Cstl_CharStream_append(
    Cstl_CharStream mut* const self, Cstl_String mut* const buf
) {
    for (char mut cur_sym = self->get(self->src_ptr)
        ; !self->is_delim(cur_sym)
            && !Cstl_CharStream_is_expired(self, lit_ptr(u32, cur_sym))
        ; cur_sym = self->get(self->src_ptr)
    ) {
        Cstl_String_push_ascii(buf, cur_sym);
    }
}

bool Cstl_CharStream_is_expired(Addr const self, Addr const ret_ptr) {
    return ((Cstl_CharStream const*) self)->is_expired(*(u32 const*) ret_ptr);
}

void Cstl_CharStream_scan(
    Cstl_CharStream mut* const self, Cstl_str const fmt, ...
) {
    VariadicArgs mut args;
    VariadicArgs_start(args, fmt);

    Cstl_CharStream_scan_impl(self, fmt, &mut args);

    VariadicArgs_end(args);
}

bool Cstl_CharStream_matches_once(
    Cstl_CharStream mut* const self, u32 const value
) {
    return Cstl_CharStream_next(self) == value;
}

bool Cstl_CharStream_matches(
    Cstl_CharStream mut* const self, Cstl_str const value
) {
    for (usize mut i = 0; i < value.len; ++i) {
        if (Cstl_CharStream_matches_once(self, (u32) value.ptr[i])) {
            return false;
        }
    }

    return true;
}

void Cstl_CharStream_scan_impl(
    Cstl_CharStream mut* const self, Cstl_str const fmt,
    VariadicArgs mut* const args
) {
    Cstl_str mut cur_match_str = Cstl_str_DEFAULT;

    Cstl_String mut buf = Cstl_String_with_capacity(64);

    for (usize mut i = 0; i < fmt.len; ++i) {
        switch (fmt.ptr[i]) {
        case ' ': {
            // match `fmt`
            {
                assert_fmt(
                    Cstl_CharStream_matches(self, cur_match_str),
                    "failed to match '{str}'",
                    cur_match_str
                );

                cur_match_str = Cstl_str_DEFAULT;
            }

            self->is_skipped = Cstl_IsSkippedFn_SPACE_SYMBOLS;
        } break;

        case '{': {
            self->is_skipped = Cstl_IsSkippedFn_DEFAULT;

            // match `fmt` substring
            {
                assert_fmt(
                    Cstl_CharStream_matches(self, cur_match_str),
                    "failed to match '{str}'",
                    cur_match_str
                );

                cur_match_str = Cstl_str_DEFAULT;
            }

            assert_msg(
                i + 1 < fmt.len,
                "incomplete format/escape scope at '{'"
            );

            if ('{' == fmt.ptr[i + 1]) {
                assert_msg(
                    Cstl_CharStream_matches_once(self, U'{'),
                    "failed to match '{'"
                );
            } else {
                Cstl_str mut fmt_scope = {
                    .ptr = fmt.ptr + i + 1,
                    .len = 0
                };

                for (usize mut j = i + 1
                    ; j < fmt.len && '}' != fmt.ptr[j]
                    ; ++j
                ) {
                    fmt_scope.len += 1;
                }

                Cstl_CharStream_scan_scope(self, fmt_scope, &mut buf, args);

                i += fmt_scope.len + 1;
            }
        } break;

        default: {
            self->is_skipped = Cstl_IsSkippedFn_DEFAULT;

            if (0 == cur_match_str.len) {
                cur_match_str = (Cstl_str) {
                    .ptr = fmt.ptr + i,
                    .len = 1
                };
            } else {
                cur_match_str.len += 1;
            }
        } break;
        }
    }

    Cstl_String_free(&buf);
}

void Cstl_CharStream_scan_scope(
    Cstl_CharStream mut* const self, Cstl_str mut fmt_scope,
    Cstl_String mut* const buf, VariadicArgs mut* const args
) {
    Cstl_String_clear(buf);
    Cstl_CharStream_append(self, buf);

    Cstl_str const type_name = Cstl_str_split_once(&mut fmt_scope, str(":"));

    Cstl_FormattableType const type = Cstl_FormattableType_parse(type_name);

    if (Cstl_FormattableType_Case_Generic == type.descriptor) {
        Cstl_ParseFn const parse = VariadicArgs_get(*args, Cstl_ParseFn);
        AddrMut const dst_ptr = VariadicArgs_get(*args, AddrMut);

        parse(dst_ptr, fmt_scope, to_str(*buf));

        return;
    }

#   define case_Type(Type) \
        case Cstl_BasicType_##Type: { \
            Type mut* const dst_ptr = VariadicArgs_get(*args, Type mut*); \
            Cstl_##Type##_parse(dst_ptr, fmt_scope, to_str(*buf)); \
        } break

    switch (type.type) {
    case_Type(u8);
    case_Type(i8);
    case_Type(u16);
    case_Type(i16);
    case_Type(u32);
    case_Type(i32);
    case_Type(u64);
    case_Type(i64);
    case_Type(usize);
    case_Type(isize);
    case_Type(f32);
    case_Type(f64);
    case_Type(bool);
    case_Type(char);
    case_Type(Addr);
    case_Type(AddrMut);
    case_Type(Vec);
    case_Type(Slice);
    case_Type(String);
    case_Type(str);
    case_Type(CStr);
    case_Type(CStrMut);

    case Cstl_BasicType_Invalid:
        // fallthrough
    default:
        deny_fmt("invalid BasicType value {u32}", type.type);
    }

#   undef case_Type
}



void Cstl_console_scan(StrLit const fmt_lit, ...) {
    VariadicArgs mut args;
    VariadicArgs_start(args, fmt_lit);

    Cstl_str const fmt = {
        .ptr = (u8 mut*) fmt_lit,
        .len = CStr_len(fmt_lit)
    };

    Cstl_console_scan_impl(fmt, &mut args);

    VariadicArgs_end(args);
}

void Cstl_console_scan_impl(Cstl_str const fmt, VariadicArgs mut* const args) {
    Cstl_CharStream mut stdin = Cstl_CharStream_STDIN;

    Cstl_CharStream_scan_impl(&mut stdin, fmt, args);
}



#define Cstl_impl_parse_uint_Type(Type) \
    void Cstl_##Type##_parse( \
        AddrMut const self_addr, Cstl_str const args, Cstl_str const src \
    ) { \
        u64 mut value; \
        Cstl_u64_parse(&mut value, args, src); \
        *(Type mut*) self_addr = value; \
    }

Cstl_impl_parse_uint_Type(u8);
Cstl_impl_parse_uint_Type(u16);
Cstl_impl_parse_uint_Type(u32);
Cstl_impl_parse_uint_Type(usize);



#define Cstl_impl_parse_int_Type(Type) \
    void Cstl_##Type##_parse( \
        AddrMut const self_addr, Cstl_str const args, Cstl_str const src \
    ) { \
        i64 mut value; \
        Cstl_u64_parse(&mut value, args, src); \
        *(Type mut*) self_addr = value; \
    }

Cstl_impl_parse_int_Type(i8);
Cstl_impl_parse_int_Type(i16);
Cstl_impl_parse_int_Type(i32);
Cstl_impl_parse_int_Type(isize);



void Cstl_u64_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    u64 mut* self = self_addr;
    *self = 0;

    for (usize mut i = 0; i < src.len; ++i) {
        *self *= 10;
        *self += src.ptr[i] - '0';
    }
}

void Cstl_i64_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    i64 mut* self = self_addr;
    *self = 0;

    bool const is_negative = args.ptr[0] == '-';

    for (usize mut i = 1; i < args.len; ++i) {
        *self *= 10;
        *self += src.ptr[i] - '0';
    }

    if (is_negative) {
        *self *= -1;
    }
}



void Cstl_f32_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    f64 mut value;
    Cstl_f64_parse(&mut value, args, src);
    *(f32 mut*) self_addr = value;
}

void Cstl_f64_parse(
    UNUSED AddrMut const self_addr, UNUSED Cstl_str const args, UNUSED Cstl_str const src
) {
    todo("f64_parse");
}

void Cstl_bool_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    if (str_eq(src, str("true"))) {
        *(bool mut*) self_addr = true;
        return;
    }

    if (str_eq(src, str("false"))) {
        *(bool mut*) self_addr = false;
        return;
    }

    deny_fmt("invalid bool_parse source '{str}'", src);
}

void Cstl_char_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    assert_fmt(1 == src.len, "invalid char_parse source '{str}'", src);

    *(char mut*) self_addr = (char) src.ptr[0];
}

void Cstl_AddrMut_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    Cstl_Addr_parse(self_addr, args, src);
}

void Cstl_Addr_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    assert_fmt(
        str_eq((str) { .ptr = src.ptr, .len = 2 }, str("0x")),
        "invalid Addr_parse source '{str}'",
        src
    );

    Addr mut* const self = self_addr;
    *self = null_mut;

    for (usize mut i = 2; i < src.len; ++i) {
        *(usize mut*) self *= 16;
        *(usize mut*) self += '0' <= src.ptr[i] && src.ptr[i] <= '9'
            ? src.ptr[i] - '0'
            : src.ptr[i] - 'a' + 10;
    }
}

void Cstl_Vec_parse(
    AddrMut const self_addr, Cstl_str mut args, Cstl_str mut src
) {
    Cstl_Vec mut* const self = self_addr;
    Cstl_Vec_clear(self);

    Cstl_str const delim_format = Cstl_str_split_once(&mut args, str(":"));
    Cstl_str const type_name = Cstl_str_split_once(&mut args, str(":"));
    Cstl_str const type_args = args;

    Cstl_str mut left_bracket = str("[");
    Cstl_str mut right_bracket = str("]");
    Cstl_str mut delim = str(", ");

    if (0 != delim_format.len) {
        Cstl_str mut format = delim_format;

        left_bracket = Cstl_str_split_once(&mut format, str("*"));
        delim = Cstl_str_split_once(&mut format, str("*"));
        right_bracket = format;
    }

    Cstl_FormattableType const type = Cstl_FormattableType_parse(type_name);

    if (Cstl_FormattableType_Case_Generic == type.descriptor) {
        todo("generic type formatters to parse arrays");
    }

    usize const elem_size = Cstl_BasicType_size(type.type);

    Cstl_Vec_retype(self, elem_size);

    Cstl_str_split_once(&mut src, left_bracket);

#   define case_Type(Type) \
        case Cstl_BasicType_##Type: { \
            Cstl_Split mut split = Cstl_str_split(src, delim); \
            for (Cstl_str mut elem_src = Cstl_Split_next(&mut split) \
                ; !Cstl_Split_is_expired(&elem_src) \
                ; elem_src = Cstl_Split_next(&mut split) \
            ) { \
                Type mut value; \
                Cstl_##Type##_parse(&mut value, type_args, elem_src); \
                Vec_push(self, &value); \
            } \
        } break
    
    switch (type.type) {
    case_Type(u8);
    case_Type(i8);
    case_Type(u16);
    case_Type(i16);
    case_Type(u32);
    case_Type(i32);
    case_Type(u64);
    case_Type(i64);
    case_Type(f32);
    case_Type(f64);
    case_Type(usize);
    case_Type(isize);
    case_Type(bool);
    case_Type(char);
    case_Type(Addr);
    case_Type(AddrMut);
    case_Type(Vec);
    case_Type(Slice);
    case_Type(String);
    case_Type(str);
    case_Type(CStr);
    case_Type(CStrMut);

    default:
        deny_fmt("invalid BasicType value {u32}", (u32) type.type);
    }

#   undef case_Type

    Cstl_str_split_once(&mut src, right_bracket);
}

void Cstl_Slice_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    todo("Slice::parse");
}

void Cstl_String_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    todo("String::parse");
}

void Cstl_str_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    todo("str::parse");
}

void Cstl_CStr_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    todo("CStr::parse");
}

void Cstl_CStrMut_parse(
    AddrMut const self_addr, Cstl_str const args, Cstl_str const src
) {
    Cstl_CStr_parse(self_addr, args, src);
}