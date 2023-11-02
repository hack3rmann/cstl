#ifndef _CSTL_TYPES_H_
#define _CSTL_TYPES_H_

#include <stddef.h>



typedef enum Bool {
    False = 0,
    True = 1
} Bool;

typedef signed char        i8;
typedef unsigned char      u8;
typedef short              i16;
typedef unsigned short     u16;
typedef int                i32;
typedef unsigned           u32;
typedef long long          i64;
typedef unsigned long long u64;

typedef size_t     usize;
typedef ptrdiff_t  isize;

#ifdef _WIN64
#   define isize_MIN  ((isize) i64_MIN)
#   define isize_MAX  ((isize) i64_MAX)
#   define usize_MIN  ((usize) u64_MIN)
#   define usize_MAX  ((usize) u64_MAX)
#else
#   define isize_MIN  ((isize) i32_MIN)
#   define isize_MAX  ((isize) i32_MAX)
#   define usize_MIN  ((usize) u32_MIN)
#   define usize_MAX  ((usize) u32_MAX)
#endif

typedef float  f32;
typedef double f64;

#define i8_MIN     ((i8) -128)
#define i8_MAX     ((i8) 127)
#define u8_MIN     ((u8) 0)
#define u8_MAX     ((u8) 0xFF)

#define i16_MIN    ((i16) -32768)
#define i16_MAX    ((i16) 32767)
#define u16_MIN    ((u16) 0)
#define u16_MAX    ((u16) 0xFFFF)

#define i32_MIN    ((i32) -2147483648)
#define i32_MAX    ((i32) 2147483647)
#define u32_MIN    ((u32) 0)
#define u32_MAX    ((u32) 0xffffffff)

#define i64_MIN    ((i64) -9223372036854775808)
#define i64_MAX    ((i64) 9223372036854775807)
#define u64_MIN    ((u64) 0)
#define u64_MAX    ((u64) 0xffffffffffffffff)



typedef char const* StrLit;

typedef void const* Addr;
typedef void* AddrMut;



#endif // !_CSTL_TYPES_H_