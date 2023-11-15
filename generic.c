#define USING_NAMESPACE_CSTL

#include "generic.h"
#include "string.h"
#include "format.h"
#include "error.h"



Cstl_BasicType Cstl_BasicType_parse(Cstl_str const value) {
    if (str_eq(value, str("u8")))       return Cstl_BasicType_u8;
    if (str_eq(value, str("i8")))       return Cstl_BasicType_i8;
    if (str_eq(value, str("u16")))      return Cstl_BasicType_u16;
    if (str_eq(value, str("i16")))      return Cstl_BasicType_i16;
    if (str_eq(value, str("u32")))      return Cstl_BasicType_u32;
    if (str_eq(value, str("i32")))      return Cstl_BasicType_i32;
    if (str_eq(value, str("u64")))      return Cstl_BasicType_u64;
    if (str_eq(value, str("i64")))      return Cstl_BasicType_i64;
    if (str_eq(value, str("f32")))      return Cstl_BasicType_f32;
    if (str_eq(value, str("f64")))      return Cstl_BasicType_f64;
    if (str_eq(value, str("Bool")))     return Cstl_BasicType_Bool;
    if (str_eq(value, str("usize")))    return Cstl_BasicType_usize;
    if (str_eq(value, str("isize")))    return Cstl_BasicType_isize;
    if (str_eq(value, str("char")))     return Cstl_BasicType_char;
    if (str_eq(value, str("Addr")))     return Cstl_BasicType_Addr;
    if (str_eq(value, str("AddrMut")))  return Cstl_BasicType_AddrMut;
    if (str_eq(value, str("Vec")))      return Cstl_BasicType_Vec;
    if (str_eq(value, str("Slice")))    return Cstl_BasicType_Slice;
    if (str_eq(value, str("String")))   return Cstl_BasicType_String;
    if (str_eq(value, str("str")))      return Cstl_BasicType_str;
    if (str_eq(value, str("CStr")))     return Cstl_BasicType_CStr;
    if (str_eq(value, str("CStrMut")))  return Cstl_BasicType_CStrMut;
    if (str_eq(value, str("StrLit")))   return Cstl_BasicType_StrLit;

    return Cstl_BasicType_Invalid;
}

void Cstl_BasicType_dbg(
    Cstl_String mut* const buf, Cstl_str const fmt, Addr const value_ptr
) {
    Cstl_BasicType const value = *(Cstl_BasicType const*) value_ptr;

    Cstl_String_append(buf, str("BasicType_"));
    Cstl_String_append(buf, Cstl_BasicType_as_str(value));
}

Cstl_str Cstl_BasicType_as_str(Cstl_BasicType const self) {
    switch (self) {
    case Cstl_BasicType_u8:         return str("u8");
    case Cstl_BasicType_i8:         return str("i8");
    case Cstl_BasicType_u16:        return str("u16");
    case Cstl_BasicType_i16:        return str("i16");
    case Cstl_BasicType_u32:        return str("u32");
    case Cstl_BasicType_i32:        return str("i32");
    case Cstl_BasicType_u64:        return str("u64");
    case Cstl_BasicType_i64:        return str("i64");
    case Cstl_BasicType_f32:        return str("f32");
    case Cstl_BasicType_f64:        return str("f64");
    case Cstl_BasicType_Bool:       return str("Bool");
    case Cstl_BasicType_usize:      return str("usize");
    case Cstl_BasicType_isize:      return str("isize");
    case Cstl_BasicType_char:       return str("char");
    case Cstl_BasicType_Addr:       return str("Addr");
    case Cstl_BasicType_AddrMut:    return str("AddrMut");
    case Cstl_BasicType_Vec:        return str("Vec");
    case Cstl_BasicType_Slice:      return str("Slice");
    case Cstl_BasicType_String:     return str("String");
    case Cstl_BasicType_str:        return str("str");
    case Cstl_BasicType_CStr:       return str("CStr");
    case Cstl_BasicType_CStrMut:    return str("CStrMut");
    case Cstl_BasicType_StrLit:     return str("StrLit");
    case Cstl_BasicType_Invalid:    return str("Invalid");
    
    default:
        Cstl_deny_fmt("invalid enum BasicType value {u32}", (u32) self);
    }
}