#ifndef _CSTL_STRING_H_
#define _CSTL_STRING_H_

#include "types.h"
#include "slice.h"
#include "util.h"



#define Cstl_String_from_literal(lit) \
    Cstl_String_from_utf8_unchecked((u8*) lit, sizeof(lit) - 1)

#define Cstl_str_from_literal(lit) \
    Cstl_str_from_utf8_unchecked((u8*) lit, sizeof(lit) - 1)



enum {
    /// `0b10xxxxxx`
    Cstl_String_UTF8_TAIL_BYTE_ENTRY = (u8) 128,
    Cstl_String_UTF8_TAIL_BYTE_MASK = (u8) 63,

    /// `0b0xxxxxxx`
    Cstl_String_UTF8_1_BYTE_ENTRY = (u8) 0,
    Cstl_String_UTF8_1_BYTE_MASK = (u8) 127,

    /// `110xxxxx`
    Cstl_String_UTF8_2_BYTE_ENTRY = (u8) (128 + 64 + 0),
    Cstl_String_UTF8_2_BYTE_MASK = (u8) 31,

    /// `1110xxxx`
    Cstl_String_UTF8_3_BYTE_ENTRY = (u8) (128 + 64 + 32 + 0),
    Cstl_String_UTF8_3_BYTE_MASK = (u8) 15,

    /// `11110xxx`
    Cstl_String_UTF8_4_BYTE_ENTRY = (u8) (128 + 64 + 32 + 16 + 0),
    Cstl_String_UTF8_4_BYTE_MASK = (u8) 7
};



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

Cstl_str Cstl_String_as_str(Cstl_String mut* self);

Cstl_Chars Cstl_String_chars(Cstl_String const* self);

Bool Cstl_String_eq(Cstl_String const* self, Cstl_String const* value);

Bool Cstl_String_ne(Cstl_String const* self, Cstl_String const* value);

Cstl_String Cstl_String_concat(usize n_strings, ...);



#ifdef USING_NAMESPACE_CSTL

    #define str_from_literal(lit) \
        Cstl_str_from_literal(lit)

    #define String_from_literal(lit) \
        Cstl_String_from_literal(lit)

    #define str(lit) \
        str_from_literal(lit)

    #define String(lit) \
        String_from_literal(lit)



    typedef Cstl_Char Char;

    #define Char_encode_utf8(self) \
        Cstl_Char_encode_utf8(self)
    
    #define Char_from_code(code) \
        Cstl_Char_from_code(code)



    typedef Cstl_str str;

    #define str_DEFAULT \
        Cstl_str_DEFAULT

    #define str_from_utf8_unchecked(ptr, len) \
        Cstl_str_from_utf8_unchecked(ptr, len)

    #define str_eq(self, value) \
        Cstl_str_eq(self, value)

    #define str_split_one(self, delim) \
        Cstl_str_split_one(self, delim)

    #define str_print(self) \
        Cstl_str_print(self)

    #define str_debug(self) \
        Cstl_str_debug(self)



    typedef Cstl_Split Split;

    #define str_split(self, delim) \
        Cstl_str_split(self, delim)

    #define Split_next(self) \
        Cstl_Split_next(self)

    #define Split_is_expired(ret) \
        Cstl_Split_is_expired(ret)



    typedef Cstl_SplitAny SplitAny;

    #define str_split_any(self, delims) \
        Cstl_str_split_any(self, delims)

    #define Split_next(self) \
        Cstl_Split_next(self)

    #define Split_is_expired(ret) \
        Cstl_Split_is_expired(ret)



    typedef Cstl_SplitWhitespace SplitWhitespace;

    #define str_split_whitespace(self) \
        Cstl_str_split_whitespace(self)

    #define SplitWhitespace_next(self) \
        Cstl_SplitWhitespace_next(self)

    #define SplitWhitespace_is_expired(ret) \
        Cstl_Cstl_SplitWhitespace_is_expired(ret)



    #define char_is_ascii_letter(self) \
        Cstl_char_is_ascii_letter(self)

    #define char_is_numeric_letter(self) \
        Cstl_char_is_numeric_letter(self)

    #define char_is_whitespace(self) \
        Cstl_char_is_whitespace(self)



    typedef Cstl_Chars Chars;

    #define str_chars(self) \
        Cstl_str_chars(self)

    #define Chars_next(self) \
        Cstl_Chars_next(self)

    #define Chars_is_expired(ret) \
        Cstl_Chars_is_expired(ret)



    typedef Cstl_String String;

    #define String_DEFAULT \
        Cstl_String_DEFAULT

    #define String_with_capacity(cap) \
        Cstl_String_with_capacity(cap)

    #define String_new() \
        Cstl_String_new()

    #define String_from_str(value) \
        Cstl_String_from_str(value)

    #define String_clone(self) \
        Cstl_String_clone(self)

    #define String_clone_from(self, src) \
        Cstl_String_clone_from(self, src)

    #define String_from_raw_c_string(str, len) \
        Cstl_String_from_raw_c_string(str, len)

    #define String_from_utf8_unchecked(bytes, n_bytes) \
        Cstl_String_from_utf8_unchecked(bytes, n_bytes)

    #define String_from_utf8(bytes, n_bytes) \
        Cstl_String_from_utf8(bytes, n_bytes)
    
    #define String_free(self) \
        Cstl_String_free(self)

    #define _internal_String_alloc(self, cap) \
        Cstl__internal_String_alloc(self, cap)

    #define _internal_String_next_capacity(cur_cap) \
        Cstl__internal_String_next_capacity(cur_cap)

    #define String_push(self, symbol) \
        Cstl_String_push(self, symbol)

    #define String_push_ascii(self, symbol) \
        Cstl_String_push_ascii(self, symbol)

    #define String_push_byte(self, byte) \
        Cstl_String_push_bytes(self, byte)

    #define String_pop(self) \
        Cstl_String_pop(self)
    
    #define String_shrink_to(self, cap) \
        Cstl_String_shrink_to(self, cap)

    #define String_shrink_to_fit(self) \
        Cstl_String_shrink_to_fit(self)

    #define String_append(self, src) \
        Cstl_String_append(self, src)

    #define String_append_bytes(self, bytes) \
        Cstl_String_append_bytes(self, bytes)

    #define String_reserve_exact(self, additional_cap) \
        Cstl_String_reserve_exact(self, additional_cap)

    #define String_reserve(self, additional_cap) \
        Cstl_String_reserve(self, additional_cap)

    #define String_as_str(self) \
        Cstl_String_as_str(self)

    #define String_chars(self) \
        Cstl_String_chars(self)

    #define String_eq(self, value) \
        Cstl_String_eq(self, value)

    #define String_ne(self, value) \
        Cstl_String_ne(self, value)

    #define String_concat(n_strings, args...) \
        Cstl_String_concat(n_string, args)

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_STRING_H_