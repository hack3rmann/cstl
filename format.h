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



typedef enum {
    Cstl_IntFmtFlags_Sign = 0x1,
    Cstl_IntFmtFlags_Case = 0x2,
    Cstl_IntFmtFlags_Default = 0x0
} Cstl_IntFmtFlags;



void Cstl_i64_fmt_impl(Cstl_String mut* buf, i64 value, u16 radix, u32 flags);



#define Cstl_declare_fmt_Type_fn(Type) \
    void Cstl_ ## Type ## _fmt( \
        Cstl_String mut* buf, Cstl_str fmt, void const* value_ptr \
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
Cstl_declare_fmt_Type_fn(Vec);
Cstl_declare_fmt_Type_fn(Slice);
Cstl_declare_fmt_Type_fn(String);
Cstl_declare_fmt_Type_fn(str);


typedef enum {
    Cstl_BasicType_u8,
    Cstl_BasicType_i8,
    Cstl_BasicType_u16,
    Cstl_BasicType_i16,
    Cstl_BasicType_u32,
    Cstl_BasicType_i32,
    Cstl_BasicType_u64,
    Cstl_BasicType_i64,
    Cstl_BasicType_f32,
    Cstl_BasicType_f64,
    Cstl_BasicType_usize,
    Cstl_BasicType_isize,
    Cstl_BasicType_char,
    Cstl_BasicType_Addr,
    Cstl_BasicType_Vec,
    Cstl_BasicType_Slice,
    Cstl_BasicType_String,
    Cstl_BasicType_str,
    Cstl_BasicType_Invalid
} Cstl_BasicType;

Cstl_BasicType Cstl_BasicType_from_str(Cstl_str value);



typedef void (*Cstl_FormatFn)(Cstl_String mut*, Cstl_str, void const*);

Cstl_FormatFn Cstl_FormatFn_from_type_name(Cstl_str type_name);

Cstl_FormatFn Cstl_FormatFn_from_basic_type(Cstl_BasicType type);



void Cstl_format_args(Cstl_String mut* buf, StrLit fmt, ...);

void Cstl_format_args_impl(
    Cstl_String mut* buf, StrLit fmt, VariadicArgs mut* args
);

Cstl_String Cstl_format(StrLit fmt, ...);

void Cstl_format_scope(
    Cstl_String mut* buf,
    Cstl_str fmt,
    void const* value_ptr,
    Cstl_FormatFn mut* format_fn
);



#ifdef USING_NAMESPACE_CSTL

    typedef Cstl_IntFmtFlags IntFmtFlags;

    #define i64_fmt_impl(buf, value, radix, flags) \
        Cstl_i64_fmt_impl(buf, value, radix, flags)

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
    #define f32_fmt Cstl_f32_fmt
    #define f64_fmt Cstl_f64_fmt
    #define Vec_fmt Cstl_Vec_fmt
    #define Slice_fmt Cstl_Slice_fmt
    #define String_fmt Cstl_String_fmt
    #define str_fmt Cstl_str_fmt



    typedef Cstl_BasicType BasicType;

    #define BasicType_u8        Cstl_BasicType_u8
    #define BasicType_i8        Cstl_BasicType_i8
    #define BasicType_u16       Cstl_BasicType_u16
    #define BasicType_i16       Cstl_BasicType_i16
    #define BasicType_u32       Cstl_BasicType_u32
    #define BasicType_i32       Cstl_BasicType_i32
    #define BasicType_u64       Cstl_BasicType_u64
    #define BasicType_i64       Cstl_BasicType_i64
    #define BasicType_usize     Cstl_BasicType_usize
    #define BasicType_isize     Cstl_BasicType_isize
    #define BasicType_Addr      Cstl_BasicType_Addr
    #define BasicType_char      Cstl_BasicType_char
    #define BasicType_Vec       Cstl_BasicType_Vec
    #define BasicType_Slice     Cstl_BasicType_Slice
    #define BasicType_String    Cstl_BasicType_String
    #define BasicType_str       Cstl_BasicType_str
    #define BasicType_Invalid   Cstl_BasicType_Invalid

    #define BasicType_from_str(value) \
        Cstl_BasicType_from_str(value)



    typedef Cstl_FormatFn FormatFn;

    #define FormatFn_from_type_name(value) \
        Cstl_FormatFn_from_type_name(value)

    #define FormatFn_from_basic_type(value) \
        Cstl_FormatFn_from_basic_type(value)



    #define format_args(buf, fmt, args...) \
        Cstl_format_args(buf, fmt, args)

    #define format_args_impl(buf, fmt, args) \
        Cstl_format_args_impl(buf, fmt, args)

    #define format(fmt, args...) \
        Cstl_format(fmt, args)

    #define format_scope(buf, fmt, value_ptr, format_fn) \
        Cstl_format_scope(buf, fmt, value_ptr, format_fn)

#endif // USING_NAMESPACE_CSTL



#endif // !_CSTL_FORMAT_H_