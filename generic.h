#ifndef _CSTL_GENERIC_H_
#define _CSTL_GENERIC_H_



#include "string.h"



typedef enum Cstl_BasicType {
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
    Cstl_BasicType_bool,
    Cstl_BasicType_usize,
    Cstl_BasicType_isize,
    Cstl_BasicType_char,
    Cstl_BasicType_Addr,
    Cstl_BasicType_AddrMut,
    Cstl_BasicType_Vec,
    Cstl_BasicType_Slice,
    Cstl_BasicType_String,
    Cstl_BasicType_str,
    Cstl_BasicType_CStr,
    Cstl_BasicType_CStrMut,
    Cstl_BasicType_StrLit,
    Cstl_BasicType_Invalid
} Cstl_BasicType;

#define Cstl_BasicType_DEFAULT Cstl_BasicType_i32

#define Cstl_BasicType_from_value(value) \
    _Generic( \
        value, \
        u8: Cstl_BasicType_u8, \
        i8: Cstl_BasicType_i8, \
        u16: Cstl_BasicType_u16, \
        i16: Cstl_BasicType_i16, \
        u32: Cstl_BasicType_u32, \
        i32: Cstl_BasicType_i32, \
        u64: Cstl_BasicType_u64, \
        i64: Cstl_BasicType_i64, \
        f32: Cstl_BasicType_f32, \
        f64: Cstl_BasicType_f64, \
        char: Cstl_BasicType_char, \
        Addr: Cstl_BasicType_Addr, \
        AddrMut: Cstl_BasicType_AddrMut, \
        Cstl_Vec: Cstl_BasicType_Vec, \
        Cstl_Slice: Cstl_BasicType_Slice, \
        Cstl_String: Cstl_BasicType_String, \
        Cstl_str: Cstl_BasicType_str, \
        CStr: Cstl_BasicType_CStr, \
        CStrMut: Cstl_BasicType_CStrMut \
    )

Cstl_BasicType Cstl_BasicType_parse(Cstl_str value);

void Cstl_BasicType_dbg(
    Cstl_String mut* buf, Cstl_str fmt, Addr value_ptr
);

Cstl_str Cstl_BasicType_as_str(Cstl_BasicType self);



#ifdef USING_NAMESPACE_CSTL

    typedef Cstl_BasicType BasicType;

    #define BasicType_DEFAULT Cstl_BasicType_DEFAULT

    #define BasicType_u8        Cstl_BasicType_u8
    #define BasicType_i8        Cstl_BasicType_i8
    #define BasicType_u16       Cstl_BasicType_u16
    #define BasicType_i16       Cstl_BasicType_i16
    #define BasicType_u32       Cstl_BasicType_u32
    #define BasicType_i32       Cstl_BasicType_i32
    #define BasicType_u64       Cstl_BasicType_u64
    #define BasicType_i64       Cstl_BasicType_i64
    #define BasicType_f32       Cstl_BasicType_f32
    #define BasicType_f64       Cstl_BasicType_f64
    #define BasicType_bool      Cstl_BasicType_bool
    #define BasicType_usize     Cstl_BasicType_usize
    #define BasicType_isize     Cstl_BasicType_isize
    #define BasicType_char      Cstl_BasicType_char
    #define BasicType_Addr      Cstl_BasicType_Addr
    #define BasicType_AddrMut   Cstl_BasicType_AddrMut
    #define BasicType_Vec       Cstl_BasicType_Vec
    #define BasicType_Slice     Cstl_BasicType_Slice
    #define BasicType_String    Cstl_BasicType_String
    #define BasicType_str       Cstl_BasicType_str
    #define BasicType_CStr      Cstl_BasicType_CStr
    #define BasicType_CStrMut   Cstl_BassicType_CStrMut
    #define BasicType_StrLit    Cstl_BasicType_StrLit
    #define BasicType_Invalid   Cstl_BasicType_Invalid

    #define BasicType_from_value Cstl_BasicType_from_value
    #define BasicType_from_str Cstl_BasicType_from_str
    #define BasicType_dbg Cstl_BasicType_dbg
    #define BasicType_as_str Cstl_BasicType_as_str

#endif

#endif // !_CSTL_GENERIC_H_