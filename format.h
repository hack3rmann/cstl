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

    #define i8_fmt(buf, fmt, value_ptr) \
        Cstl_i8_fmt(buf, fmt, value_ptr)

    #define u8_fmt(buf, fmt, value_ptr) \
        Cstl_u8_fmt(buf, fmt, value_ptr)
    
    #define i16_fmt(buf, fmt, value_ptr) \
        Cstl_i16_fmt(buf, fmt, value_ptr)
    
    #define u16_fmt(buf, fmt, value_ptr) \
        Cstl_u16_fmt(buf, fmt, value_ptr)
    
    #define i32_fmt(buf, fmt, value_ptr) \
        Cstl_i32_fmt(buf, fmt, value_ptr)
    
    #define u32_fmt(buf, fmt, value_ptr) \
        Cstl_u32_fmt(buf, fmt, value_ptr)
    
    #define i64_fmt(buf, fmt, value_ptr) \
        Cstl_i64_fmt(buf, fmt, value_ptr)
    
    #define u64_fmt(buf, fmt, value_ptr) \
        Cstl_u64_fmt(buf, fmt, value_ptr)
    
    #define isize_fmt(buf, fmt, value_ptr) \
        Cstl_isize_fmt(buf, fmt, value_ptr)
    
    #define usize_fmt(buf, fmt, value_ptr) \
        Cstl_usize_fmt(buf, fmt, value_ptr)
    
    #define f32_fmt(buf, fmt, value_ptr) \
        Cstl_f32_fmt(buf, fmt, value_ptr)
    
    #define f64_fmt(buf, fmt, value_ptr) \
        Cstl_f64_fmt(buf, fmt, value_ptr)
    
    #define Vec_fmt(buf, fmt, value_ptr) \
        Cstl_Vec_fmt(buf, fmt, value_ptr)
    
    #define Slice_fmt(buf, fmt, value_ptr) \
        Cstl_Slice_fmt(buf, fmt, value_ptr)
    
    #define String_fmt(buf, fmt, value_ptr) \
        Cstl_String_fmt(buf, fmt, value_ptr)
    
    #define str_fmt(buf, fmt, value_ptr) \
        Cstl_str_fmt(buf, fmt, value_ptr)



    typedef Cstl_BasicType BasicType;

    #define Cstl_BasicType_u8 BasicType_u8
    #define Cstl_BasicType_i8 BasicType_i8
    #define Cstl_BasicType_u16 BasicType_u16
    #define Cstl_BasicType_i16 BasicType_i16
    #define Cstl_BasicType_u32 BasicType_u32
    #define Cstl_BasicType_i32 BasicType_i32
    #define Cstl_BasicType_u64 BasicType_u64
    #define Cstl_BasicType_i64 BasicType_i64
    #define Cstl_BasicType_usize BasicType_usize
    #define Cstl_BasicType_isize BasicType_isize
    #define Cstl_BasicType_Vec BasicType_Vec
    #define Cstl_BasicType_Slice BasicType_Slice
    #define Cstl_BasicType_String BasicType_String
    #define Cstl_BasicType_str BasicType_str
    #define Cstl_BasicType_Invalid BasicType_Invalid

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