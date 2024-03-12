#ifndef _CSTL_TYPES_H_
#define _CSTL_TYPES_H_



#define mut

#ifndef null_mut
#   define null_mut ((AddrMut) 0)
#endif

#ifndef null
#   define null ((Addr) 0)
#endif



typedef _Bool bool;

#define true ((bool) 1)
#define false ((bool) 0)

bool bool_implies(bool self, bool other);



typedef signed char schar;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;



typedef schar i8;
typedef uchar u8;
typedef short  i16;
typedef ushort u16;

#if __INT_MAX__ == 2147483647
    typedef int i32;
    typedef uint u32;
#elif __LONG_MAX__ == 2147483647
    typedef long i32;
    typedef ulong u32;
#endif

#if __LONG_LONG_MAX__ == 9223372036854775807
    typedef llong i64;
    typedef ullong u64;
#endif

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



typedef float  f32;
typedef double f64;



#if __PTRDIFF_MAX__ == 32767
    typedef u16 usize;
    typedef i16 isize;

#   define usize_MIN u16_MIN
#   define usize_MAX u16_MAX
#   define isize_MIN i16_MIN
#   define isize_MAX i16_MAX

#   define Addr_SIZE 2
#elif __PTRDIFF_MAX__ == 2147483647
    typedef u32 usize;
    typedef i32 isize;

#   define usize_MIN u32_MIN
#   define usize_MAX u32_MAX
#   define isize_MIN i32_MIN
#   define isize_MAX i32_MAX

#   define Addr_SIZE 4
#elif __PTRDIFF_MAX__ == 9223372036854775807
    typedef u64 usize;
    typedef i64 isize;

#   define usize_MIN u64_MIN
#   define usize_MAX u64_MAX
#   define isize_MIN i64_MIN
#   define isize_MAX i64_MAX

#   define Addr_SIZE 8
#endif

#define AddrMut_SIZE Addr_SIZE



typedef char const* StrLit;
typedef char const* CStr;
typedef char mut* CStrMut;



typedef void const* Addr;

#define Addr_DEFAULT null

#define Addr_NULL null

Addr Addr_from_usize(usize value);

usize Addr_as_usize(Addr self);



typedef void mut* AddrMut;

#define AddrMut_DEFAULT null_mut

#define AddrMut_NULL null_mut

AddrMut AddrMut_from_usize(usize value);

usize AddrMut_as_usize(AddrMut self);



#endif // !_CSTL_TYPES_H_