#ifndef _CSTL_TYPES_H_
#define _CSTL_TYPES_H_



#define mut



typedef enum Bool {
    False = 0,
    True = 1
} Bool;

Bool Bool_implies(Bool self, Bool other);



typedef signed char        i8;
typedef unsigned char      u8;
typedef short              i16;
typedef unsigned short     u16;

#if __INT_MAX__ == 2147483647
    typedef int i32;
    typedef unsigned u32;
#elif __LONG_MAX__ == 2147483647
    typedef long i32;
    typedef unsigned long u32;
#endif

#if __LONG_LONG_MAX__ == 9223372036854775807
    typedef long long i64;
    typedef unsigned long long u64;
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



#if __PTRDIFF_MAX__ == 32767
    typedef u16 usize;
    typedef i16 isize;

#   define usize_MIN u16_MIN
#   define usize_MAX u16_MAX
#   define isize_MIN i16_MIN
#   define isize_MAX i16_MAX
#elif __PTRDIFF_MAX__ == 2147483647
    typedef u32 usize;
    typedef i32 isize;

#   define usize_MIN u32_MIN
#   define usize_MAX u32_MAX
#   define isize_MIN i32_MIN
#   define isize_MAX i32_MAX
#elif __PTRDIFF_MAX__ == 9223372036854775807
    typedef u64 usize;
    typedef i64 isize;

#   define usize_MIN u64_MIN
#   define usize_MAX u64_MAX
#   define isize_MIN i64_MIN
#   define isize_MAX i64_MAX
#endif



typedef char const* StrLit;
typedef char const* CStr;
typedef char mut* CStrMut;

typedef void const* Addr;
typedef void mut* AddrMut;



#endif // !_CSTL_TYPES_H_