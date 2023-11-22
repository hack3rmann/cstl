#ifndef _CSTL_FORMAT_H_
#define _CSTL_FORMAT_H_

/// fmt(&mut buf, "{i32}", 32);
///     buf: "32"
/// 
/// fmt(&mut buf, "{f32:.2}", 1.11111);
///     buf: "1.11"
/// 
/// vec: [1, 2, 3, -4]
/// fmt(&mut buf, "{Vec:(a, b):i32}", &vec);
///     buf: "(1, 2, 3, -4)"
///
/// slice: [1.5, 2.12346, -3.0909, 4.1]
/// fmt(&mut buf, "{Slice:[, ]:f64:.4}", &slice);
///     buf: "[1.5000, 2.1235, -3.0909, 4.1000]"

/// (i|u)(8|16|32|64): {(i|u)(8|16|32|64):(s)(u|l)(0b|0h|0o|0xB)}
/// 
/// 0b = binary, 0h = hex, 0o = octal, 0xB = base B
/// B := 1..9(a|A)..(z|Z)
/// u = upper, l = lower
/// s = show sign (aka +1)
/// 
/// ex: {i32:su0x15} with 11: "+B"

/// f(32|64): {f(32|64):(s)(u|l)(0b|0h|0o|0xB)((+|-)(.|,)N)}
/// 
/// 0b = binary, 0h = hex, 0o = octal, 0xB = base B
/// B := 1..9(a|A)..(z|Z)
/// u = upper, l = lower
/// s = show sign (aka +1)
/// N = number of fraction digits with
///     . = dot notation
///     , = comma notation
///     + = round up
///     - = round down
///     none = round
/// 
/// ex: {f64:sl0h+,2} with 10.256: "+a,2"

/// Vec: {Vec:L*D*R:Type:TypeArgs}
/// 
/// L = left bracket (default: '[')
/// R = right bracket (default: ']')
/// D = delimiter (default: ', ')
/// 
/// ex: {Vec:(*;*]:i32:0b} with [1, 2, 3, 4, 5, 6]:
///     "(0b1;0b10;0b11;0b100;0b101;0b110]"

/// Any formattable type: {*:args}
/// 
/// looks like: fmt(&mut buf, "{*:.1}", f32_fmt, 4.55)
///     buf: "4.6"
/// 
/// void f32_fmt(String mut* buf, str fmt_args);
///     fmt_args: ".1"

#include "string.h"
#include "variadic.h"
#include "generic.h"



typedef enum Cstl_FloatNotation {
    Cstl_FloatNotation_Scientific,
    Cstl_FloatNotation_Point
} Cstl_FloatNotation;

#define Cstl_FloatNotation_DEFAULT Cstl_FloatNotation_Point



typedef struct Cstl_FloatImpl {
    i32 sign, exp;
    u64 frac;
} Cstl_FloatImpl;

#define Cstl_FloatImpl_DEFAULT ((Cstl_FloatImpl) { \
    .sign = 1, .exp = 0, .frac = 0 \
})

Cstl_FloatImpl Cstl_FloatImpl_from_f32(f32 value);

Cstl_FloatImpl Cstl_FloatImpl_from_f64(f64 value);

f32 Cstl_FloatImpl_to_f32(Cstl_FloatImpl self);

f64 Cstl_FloatImpl_to_f64(Cstl_FloatImpl self);

void Cstl_FloatImpl_dbg(Cstl_String mut* buf, Cstl_str fmt, Addr value_ptr);



typedef enum Cstl_FloatRound {
    Cstl_FloatRound_Up,
    Cstl_FloatRound_Down,
    Cstl_FloatRound_Math
} Cstl_FloatRound;

#define Cstl_FloatRound_DEFAULT Cstl_FloatRound_Math



typedef enum Cstl_IntegerFmtFlags {
    Cstl_IntegerFmtFlags_ShowSign = 0x1,
    Cstl_IntegerFmtFlags_Uppercase = 0x2,
} Cstl_IntegerFmtFlags;

#define Cstl_IntegerFmtFlags_DEFAULT 0



typedef struct Cstl_FloatFormatDescriptor {
    f64 value;
    u16 radix;
    u16 flags;
    u16 n_fraction_digits;
    char fraction_delim;
    Cstl_FloatRound round;
    Cstl_FloatNotation notation;
} Cstl_FloatFormatDescriptor;

#define Cstl_FloatFormatDescriptor_DEFAULT ((Cstl_FloatFormatDescriptor) { \
    .value = 0.0, .radix = 10, .flags = Cstl_IntegerFmtFlags_DEFAULT, \
    .n_fraction_digits = u16_MAX, .fraction_delim = '.', \
    .round = Cstl_FloatRound_DEFAULT, .notation = Cstl_FloatNotation_DEFAULT \
})

Cstl_FloatFormatDescriptor Cstl_FloatFormatDescriptor_parse(Cstl_str value);



typedef struct Cstl_IntegerFormatDescriptor {
    u64 value;
    i16 sign;
    u16 radix;
    u32 flags;
} Cstl_IntegerFormatDescriptor;

#define Cstl_IntegerFormatDescriptor_DEFAULT ((Cstl_IntegerFormatDescriptor) { \
    .value = 0, .sign = 1, .radix = 10, .flags = Cstl_IntegerFmtFlags_DEFAULT \
})

Cstl_IntegerFormatDescriptor Cstl_IntegerFormatDescriptor_parse(Cstl_str value);



void Cstl_x64_fmt_impl(Cstl_String mut* buf, Cstl_IntegerFormatDescriptor desc);

void Cstl_f64_fmt_impl(Cstl_String mut* buf, Cstl_FloatFormatDescriptor desc);



#define Cstl_declare_fmt_Type_fn(Type) \
    void Cstl_ ## Type ## _fmt( \
        Cstl_String mut* buf, Cstl_str fmt, Addr value_ptr \
    );

Cstl_declare_fmt_Type_fn(u8);
Cstl_declare_fmt_Type_fn(i8);
Cstl_declare_fmt_Type_fn(u16);
Cstl_declare_fmt_Type_fn(i16);
Cstl_declare_fmt_Type_fn(u32);
Cstl_declare_fmt_Type_fn(i32);
Cstl_declare_fmt_Type_fn(u64);
Cstl_declare_fmt_Type_fn(i64);
Cstl_declare_fmt_Type_fn(f32);
Cstl_declare_fmt_Type_fn(f64);
Cstl_declare_fmt_Type_fn(usize);
Cstl_declare_fmt_Type_fn(isize);
Cstl_declare_fmt_Type_fn(char);
Cstl_declare_fmt_Type_fn(Addr);
Cstl_declare_fmt_Type_fn(AddrMut);
Cstl_declare_fmt_Type_fn(Vec);
Cstl_declare_fmt_Type_fn(Slice);
Cstl_declare_fmt_Type_fn(String);
Cstl_declare_fmt_Type_fn(str);
Cstl_declare_fmt_Type_fn(CStr);
Cstl_declare_fmt_Type_fn(CStrMut);
Cstl_declare_fmt_Type_fn(StrLit);




typedef struct Cstl_FormattableType {
    enum Cstl_FormattableType_Case {
        Cstl_FormattableType_Case_Type,
        Cstl_FormattableType_Case_Generic
    } descriptor;

    Cstl_BasicType type;
} Cstl_FormattableType;

#define Cstl_FormattableType_DEFAULT ((Cstl_Formattable_Type) { \
    .descriptor = Cstl_FormattableType_Case_Type, \
    .type = Cstl_BasicType_DEFAULT \
})

Cstl_FormattableType Cstl_FormattableType_parse(Cstl_str value);



typedef void (*Cstl_FormatFn)(Cstl_String mut*, Cstl_str, Addr);

Cstl_FormatFn Cstl_FormatFn_from_type_name(Cstl_str type_name);

Cstl_FormatFn Cstl_FormatFn_from_formattable_type(Cstl_FormattableType type);



void Cstl_format_args(Cstl_String mut* buf, StrLit fmt, ...);

void Cstl_format_args_impl(
    Cstl_String mut* buf, StrLit fmt, VariadicArgs mut* args
);

Cstl_String Cstl_format(StrLit fmt, ...);

void Cstl_format_scope(
    Cstl_String mut* buf,
    Cstl_str fmt,
    Addr value_ptr,
    Cstl_FormatFn mut* format_fn
);



#ifdef USING_NAMESPACE_CSTL

    typedef Cstl_FloatNotation FloatNotation;

    #define FloatNotation_Scientific Cstl_FloatNotation_Specific
    #define FloatNotation_Point Cstl_FloatNotation_Point

    #define FloatNotation_DEFAULT Cstl_FloatNotation_DEFAULT



    typedef Cstl_FloatImpl FloatImpl;

    #define FloatImpl_DEFAULT Cstl_FloatImpl_DEFAULT

    #define FloatImpl_from_f32 Cstl_FloatImpl_from_f32
    #define FloatImpl_to_f32 Cstl_FloatImpl_to_f32
    #define FloatImpl_from_f64 Cstl_FloatImpl_from_f64
    #define FloatImpl_to_f64 Cstl_FloatImpl_to_f64
    #define FloatImpl_dbg Cstl_FloatImpl_dbg



    typedef Cstl_FloatRound Round;

    #define Round_Up   Cstl_FloatRound_Up
    #define Round_Down Cstl_FloatRound_Down
    #define Round_Math Cstl_FloatRound_Math
    
    #define FloatRound_DEFAULT Cstl_FloatRound_DEFAULT



    typedef Cstl_IntegerFmtFlags IntegerFmtFlags;

    #define IntegerFmtFlags_DEFAULT Cstl_IntegerFmtFlags_DEFAULT



    typedef Cstl_FloatFormatDescriptor FloatFormatDescriptor;

    #define FloatFormatDescriptor_DEFAULT Cstl_FloatFormatDescriptor_DEFAULT
    
    #define FloatFormatDescriptor_parse Cstl_FloatFormatDescriptor_parse



    typedef Cstl_IntegerFormatDescriptor IntegerFormatDescriptor;

    #define IntegerFormatDescriptor_DEFAULT Cstl_IntegerFormatDescriptor_DEFAULT

    #define IntegerFormatDescriptor_parse Cstl_IntegerFormatDescriptor_parse



    #define x64_fmt_impl Cstl_x64_fmt_impl
    #define f64_fmt_impl Cstl_x64_fmt_impl



    #define i8_fmt Cstl_i8_fmt
    #define u8_fmt Cstl_u8_fmt
    #define i16_fmt Cstl_i16_fmt
    #define u16_fmt Cstl_u16_fmt
    #define i32_fmt Cstl_i32_fmt
    #define u32_fmt Cstl_u32_fmt
    #define i64_fmt Cstl_i64_fmt
    #define u64_fmt Cstl_u64_fmt
    #define isize_fmt Cstl_isize_fmt
    #define usize_fmt Cstl_usize_fmt
    #define bool_fmt Cstl_bool_fmt
    #define Addr_fmt Cstl_Addr_fmt
    #define AddrMut_fmt Cstl_AddrMut_fmt
    #define char_fmt Cstl_char_fmt
    #define f32_fmt Cstl_f32_fmt
    #define f64_fmt Cstl_f64_fmt
    #define Vec_fmt Cstl_Vec_fmt
    #define Slice_fmt Cstl_Slice_fmt
    #define String_fmt Cstl_String_fmt
    #define str_fmt Cstl_str_fmt
    #define CStr_fmt Cstl_CStr_fmt
    #define CStrMut_fmt Cstl_CStrMut_fmt



    #define FormattableType Cstl_FCstl_FormattableType

    #define FormattableType_DEFAULT Cstl_Formattable_Type_DEFAULT

    #define FormattableType_parse Cstl_FormattableType_parse



    typedef Cstl_FormatFn FormatFn;

    #define FormatFn_from_type_name Cstl_FormatFn_from_type_name
    #define FormatFn_from_formattable_type Cstl_FormatFn_from_formattable_type



    #define format_args Cstl_format_args
    #define format_args_impl Cstl_format_args_impl
    #define format Cstl_format
    #define format_scope Cstl_format_scope

#endif // USING_NAMESPACE_CSTL



#endif // !_CSTL_FORMAT_H_