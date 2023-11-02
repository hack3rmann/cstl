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
    Cstl_String_utf8_tail_byte_entry = (u8) 128,
    Cstl_String_utf8_tail_byte_mask = (u8) 63,

    /// `0b0xxxxxxx`
    Cstl_String_utf8_1_byte_entry = (u8) 0,
    Cstl_String_utf8_1_byte_mask = (u8) 127,

    /// `110xxxxx`
    Cstl_String_utf8_2_byte_entry = (u8) (128 + 64 + 0),
    Cstl_String_utf8_2_byte_mask = (u8) 31,

    /// `1110xxxx`
    Cstl_String_utf8_3_byte_entry = (u8) (128 + 64 + 32 + 0),
    Cstl_String_utf8_3_byte_mask = (u8) 15,

    /// `11110xxx`
    Cstl_String_utf8_4_byte_entry = (u8) (128 + 64 + 32 + 16 + 0),
    Cstl_String_utf8_4_byte_mask = (u8) 7
};



typedef enum {
    Cstl_Utf8ByteClassification_SingleByte,
    Cstl_Utf8ByteClassification_PairByteEntry,
    Cstl_Utf8ByteClassification_TripleByteEntry,
    Cstl_Utf8ByteClassification_QuadByteEntry,
    Cstl_Utf8ByteClassification_TailByte,
    Cstl_Utf8ByteClassification_InvalidByte
} Cstl_Utf8ByteClassification;

Cstl_Utf8ByteClassification Cstl_Utf8ByteClassification_of(u8 byte);

Bool Cstl_Utf8ByteClassification_is_single_byte(u8 byte);

Bool Cstl_Utf8ByteClassification_is_pair_byte_entry(u8 byte);

Bool Cstl_Utf8ByteClassification_is_triple_byte_entry(u8 byte);

Bool Cstl_Utf8ByteClassification_is_quad_byte_entry(u8 byte);

Bool Cstl_Utf8ByteClassification_is_tail_byte(u8 byte);

Bool Cstl_Utf8ByteClassification_is_invalid_byte(u8 byte);

usize Cstl_Utf8ByteClassification_size(Cstl_Utf8ByteClassification self);



typedef u32 Cstl_Char;

u32 Cstl_Char_encode_utf8(Cstl_Char self);

Cstl_Char Cstl_Char_from_code(u32 code);



void Cstl_set_utf8_output_encoding(void);



typedef struct {
    u8* ptr;
    usize len;
} Cstl_str;

extern Cstl_str const Cstl_str_DEFAULT;

Cstl_str Cstl_str_from_utf8_unchecked(u8 mut* ptr, usize len);

Bool Cstl_str_eq(Cstl_str self, Cstl_str value);

Cstl_str Cstl_str_split_one(Cstl_str mut* self, Cstl_str delim);

void Cstl_str_print(Cstl_str self);

void Cstl_str_debug(Cstl_str self);



typedef struct {
    Cstl_str string;
    Cstl_str delim;
} Cstl_Split;

Cstl_Split Cstl_str_split(Cstl_str self, Cstl_str delim);

Cstl_str Cstl_Split_next(Cstl_Split mut* self);

Bool Cstl_Split_is_expired(Cstl_str const* ret);



typedef struct {
    Cstl_str string;
    Cstl_Slice delims;
} Cstl_SplitAny;

Cstl_SplitAny Cstl_str_split_any(Cstl_str self, Cstl_Slice demims);

Cstl_str Cstl_Split_next(Cstl_Split mut* self);

Bool Cstl_Split_is_expired(Cstl_str const* ret);



typedef struct {
    Cstl_str string;
} Cstl_SplitWhitespace;

Cstl_SplitWhitespace Cstl_str_split_whitespace(Cstl_str self);

Cstl_str Cstl_SplitWhitespace_next(Cstl_SplitWhitespace mut* self);

Bool Cstl_SplitWhitespace_is_expired(Cstl_str const* ret);



Bool Cstl_char_is_ascii_letter(char self);

Bool Cstl_char_is_numeric_letter(char self);

Bool Cstl_char_is_whitespace(char self);



typedef struct {
    Cstl_str string;
} Cstl_Chars;

Cstl_Chars Cstl_str_chars(Cstl_str self);

Cstl_Char Cstl_Chars_next(Cstl_Chars mut* self);

Bool Cstl_Chars_is_expired(Cstl_Char const* ret);



typedef struct {
    u8* ptr;
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

void Cstl_String_free(Cstl_String mut* self);

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

    typedef Cstl_String String;
    typedef Cstl_str str;

#endif // USING_NAMESPACE_CSTL



#endif // !_CSTL_STRING_H_