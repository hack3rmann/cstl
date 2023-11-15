#ifndef _CSTL_STRING_H_
#define _CSTL_STRING_H_

#include "types.h"
#include "slice.h"
#include "util.h"



#define Cstl_String_from_literal(lit) \
    Cstl_String_from_utf8_unchecked((u8 mut*) lit, sizeof(lit) - 1)

#define Cstl_str_from_literal(lit) \
    Cstl_str_from_utf8_unchecked((u8 mut*) lit, sizeof(lit) - 1)

#define Cstl_to_str(value) \
    ((union { \
        typeof(value) value_field; \
        Cstl_str string; \
    }) { .value_field = value }.string)

#define Cstl_as_str(value_ptr) \
    ((union { \
        typeof(value_ptr) value_field_ptr; \
        Cstl_str mut* string_ptr; \
    }) { .value_field_ptr = value_ptr }.string_ptr)



/// `0b10xxxxxx`
#define Cstl_String_UTF8_TAIL_BYTE_ENTRY (u8) 128
#define Cstl_String_UTF8_TAIL_BYTE_MASK (u8) 63

/// `0b0xxxxxxx`
#define Cstl_String_UTF8_1_BYTE_ENTRY (u8) 0
#define Cstl_String_UTF8_1_BYTE_MASK (u8) 127

/// `110xxxxx`
#define Cstl_String_UTF8_2_BYTE_ENTRY (u8) (128 + 64 + 0)
#define Cstl_String_UTF8_2_BYTE_MASK (u8) 31

/// `1110xxxx`
#define Cstl_String_UTF8_3_BYTE_ENTRY (u8) (128 + 64 + 32 + 0)
#define Cstl_String_UTF8_3_BYTE_MASK (u8) 15

/// `11110xxx`
#define Cstl_String_UTF8_4_BYTE_ENTRY (u8) (128 + 64 + 32 + 16 + 0)
#define Cstl_String_UTF8_4_BYTE_MASK (u8) 7



typedef enum Cstl_Utf8ByteType {
    Cstl_Utf8ByteType_SingleByte,
    Cstl_Utf8ByteType_PairByteEntry,
    Cstl_Utf8ByteType_TripleByteEntry,
    Cstl_Utf8ByteType_QuadByteEntry,
    Cstl_Utf8ByteType_TailByte,
    Cstl_Utf8ByteType_InvalidByte
} Cstl_Utf8ByteType;

Cstl_Utf8ByteType Cstl_Utf8ByteType_of(u8 byte);

Bool Cstl_Utf8ByteType_is_single_byte(u8 byte);

Bool Cstl_Utf8ByteType_is_pair_byte_entry(u8 byte);

Bool Cstl_Utf8ByteType_is_triple_byte_entry(u8 byte);

Bool Cstl_Utf8ByteType_is_quad_byte_entry(u8 byte);

Bool Cstl_Utf8ByteType_is_tail_byte(u8 byte);

Bool Cstl_Utf8ByteType_is_invalid_byte(u8 byte);

usize Cstl_Utf8ByteType_size(Cstl_Utf8ByteType self);



typedef u32 Cstl_Char;

u32 Cstl_Char_encode_utf8(Cstl_Char self);

Cstl_Char Cstl_Char_from_code(u32 code);



void Cstl_set_utf8_output_encoding(void);



typedef struct Cstl_str {
    u8 mut* ptr;
    usize len;
} Cstl_str;

extern Cstl_str const Cstl_str_DEFAULT;

Cstl_str Cstl_str_from_utf8_unchecked(u8 mut* ptr, usize len);

Bool Cstl_str_eq(Cstl_str self, Cstl_str value);

Cstl_str Cstl_str_split_one(Cstl_str mut* self, Cstl_str delim);

void Cstl_str_print(Cstl_str self);

void Cstl_str_debug(Cstl_str self);



typedef struct Cstl_Split {
    Cstl_str string;
    Cstl_str delim;
} Cstl_Split;

Cstl_Split Cstl_str_split(Cstl_str self, Cstl_str delim);

Cstl_str Cstl_Split_next(Cstl_Split mut* self);

Bool Cstl_Split_is_expired(Cstl_str const* ret);



typedef struct Cstl_SplitAny {
    Cstl_str string;
    Cstl_Slice delims;
} Cstl_SplitAny;

Cstl_SplitAny Cstl_str_split_any(Cstl_str self, Cstl_Slice demims);

Cstl_str Cstl_Split_next(Cstl_Split mut* self);

Bool Cstl_Split_is_expired(Cstl_str const* ret);



typedef struct Cstl_SplitWhitespace {
    Cstl_str string;
} Cstl_SplitWhitespace;

Cstl_SplitWhitespace Cstl_str_split_whitespace(Cstl_str self);

Cstl_str Cstl_SplitWhitespace_next(Cstl_SplitWhitespace mut* self);

Bool Cstl_SplitWhitespace_is_expired(Cstl_str const* ret);



Bool Cstl_char_is_ascii_letter(char self);

Bool Cstl_char_is_numeric_letter(char self);

Bool Cstl_char_is_whitespace(char self);



typedef struct Cstl_Chars {
    Cstl_str string;
} Cstl_Chars;

Cstl_Chars Cstl_str_chars(Cstl_str self);

Cstl_Char Cstl_Chars_next(Cstl_Chars mut* self);

Bool Cstl_Chars_is_expired(Cstl_Char const* ret);

Cstl_Slice_u8 Cstl_str_as_bytes(Cstl_str self);



typedef struct Cstl_String {
    u8 mut* ptr;
    usize len;
    usize cap;
} Cstl_String;

extern Cstl_String const Cstl_String_DEFAULT;

Cstl_String Cstl_String_with_capacity(usize cap);

Cstl_String Cstl_String_new(void);

Cstl_String Cstl_String_from_str(Cstl_str string);

Cstl_String Cstl_String_clone(Cstl_String const* self);

void Cstl_String_clone_from(Cstl_String mut* self, Cstl_String const* src);

Cstl_String Cstl_String_from_raw_c_string(char mut* str, usize len);

Cstl_String Cstl_String_from_utf8_unchecked(u8 mut* bytes, usize n_bytes);

Cstl_String Cstl_String_from_utf8(u8 mut* bytes, usize n_bytes);

void Cstl_String_free(Cstl_String const* self);

void Cstl__internal_String_alloc(Cstl_String mut* self, usize cap);

usize Cstl__internal_String_next_capacity(usize cur_cap);

void Cstl_String_push(Cstl_String mut* self, Cstl_Char symbol);

void Cstl_String_push_ascii(Cstl_String mut* self, char symbol);

void Cstl_String_push_byte(Cstl_String mut* self, u8 value);

Cstl_Char Cstl_String_pop(Cstl_String mut* self);

void Cstl_String_shrink_to(Cstl_String mut* self, usize cap);

void Cstl_String_shrink_to_fit(Cstl_String mut* self);

void Cstl_String_append(Cstl_String mut* self, Cstl_str src);

void Cstl_String_append_bytes(Cstl_String mut* self, Cstl_Slice_u8 slice);

void Cstl_String_reserve_exact(Cstl_String mut* self, usize additional_cap);

void Cstl_String_reserve(Cstl_String mut* self, usize additional_cap);

Cstl_str Cstl_String_as_str(Cstl_String const* self);

Cstl_Chars Cstl_String_chars(Cstl_String const* self);

Bool Cstl_String_eq(Addr lhs, Addr rhs);

Bool Cstl_String_ne(Addr lhs, Addr rhs);

Cstl_String Cstl_String_concat(usize n_strings, ...);

Cstl_Slice_u8 Cstl_String_as_bytes(Cstl_String const* self);



#ifdef USING_NAMESPACE_CSTL

    #define str_from_literal Cstl_str_from_literal
    #define String_from_literal Cstl_String_from_literal

    #define str(lit) \
        str_from_literal(lit)

    #define String(lit) \
        String_from_literal(lit)

    #define to_str(value) \
        Cstl_to_str(value)

    #define as_str(value) \
        Cstl_as_str(value)



    typedef Cstl_Char Char;

    #define Char_encode_utf8 Cstl_Char_encode_utf8
    #define Char_from_code Cstl_Char_from_code



    typedef Cstl_str str;

    #define str_DEFAULT Cstl_str_DEFAULT

    #define str_from_utf8_unchecked Cstl_str_from_utf8_unchecked
    #define str_eq Cstl_str_eq
    #define str_split_one Cstl_str_split_one
    #define str_print Cstl_str_print
    #define str_debug Cstl_str_debug
    #define str_as_bytes Cstl_str_as_bytes



    typedef Cstl_Split Split;

    #define str_split Cstl_str_split
    #define Split_next Cstl_Split_next
    #define Split_is_expired Cstl_Split_is_expired



    typedef Cstl_SplitAny SplitAny;

    #define str_split_any Cstl_str_split_any
    #define Split_next Cstl_Split_next
    #define Split_is_expired Cstl_Split_is_expired



    typedef Cstl_SplitWhitespace SplitWhitespace;

    #define str_split_whitespace Cstl_str_split_whitespace
    #define SplitWhitespace_next Cstl_SplitWhitespace_next
    #define SplitWhitespace_is_expired Cstl_Cstl_SplitWhitespace_is_expired



    #define char_is_ascii_letter Cstl_char_is_ascii_letter
    #define char_is_numeric_letter Cstl_char_is_numeric_letter
    #define char_is_whitespace Cstl_char_is_whitespace



    typedef Cstl_Chars Chars;

    #define str_chars Cstl_str_chars
    #define Chars_next Cstl_Chars_next
    #define Chars_is_expired Cstl_Chars_is_expired



    typedef Cstl_String String;

    #define String_DEFAULT Cstl_String_DEFAULT

    #define String_with_capacity Cstl_String_with_capacity
    #define String_new Cstl_String_new
    #define String_from_str Cstl_String_from_str
    #define String_clone Cstl_String_clone
    #define String_clone_from Cstl_String_clone_from
    #define String_from_raw_c_string Cstl_String_from_raw_c_string
    #define String_from_utf8_unchecked Cstl_String_from_utf8_unchecked
    #define String_from_utf8 Cstl_String_from_utf8
    #define String_free Cstl_String_free
    #define _internal_String_alloc Cstl__internal_String_alloc
    #define _internal_String_next_capacity Cstl__internal_String_next_capacity
    #define String_push Cstl_String_push
    #define String_push_ascii Cstl_String_push_ascii
    #define String_push_byte Cstl_String_push_bytes
    #define String_pop Cstl_String_pop
    #define String_shrink_to Cstl_String_shrink_to
    #define String_shrink_to_fit Cstl_String_shrink_to_fit
    #define String_append Cstl_String_append
    #define String_append_bytes Cstl_String_append_bytes
    #define String_reserve_exact Cstl_String_reserve_exact
    #define String_reserve Cstl_String_reserve
    #define String_as_str Cstl_String_as_str
    #define String_chars Cstl_String_chars
    #define String_eq Cstl_String_eq
    #define String_ne Cstl_String_ne
    #define String_concat Cstl_String_concat
    #define String_as_bytes Cstl_String_as_bytes

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_STRING_H_