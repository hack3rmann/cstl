#if defined(_WIN32) && defined(_WIN64)
#   include <windows.h>
#endif

#include "string.h"
#include "error.h"
#include "util.h"
#include "compare.h"
#include "variadic.h"
#include "memory.h"
#include "io.h"



extern int putchar(int symbol);



Bool Cstl_Utf8ByteType_is_single_byte(u8 const byte) {
    return Cstl_String_UTF8_1_BYTE_ENTRY
        == (byte & ~(u8) Cstl_String_UTF8_1_BYTE_MASK & 255);
}

Bool Cstl_Utf8ByteType_is_pair_byte_entry(u8 const byte) {
    return Cstl_String_UTF8_2_BYTE_ENTRY
        == (byte & ~(u8) Cstl_String_UTF8_2_BYTE_MASK & 255);
}

Bool Cstl_Utf8ByteType_is_triple_byte_entry(u8 const byte) {
    return Cstl_String_UTF8_3_BYTE_ENTRY
        == (byte & ~(u8) Cstl_String_UTF8_3_BYTE_MASK & 255);
}

Bool Cstl_Utf8ByteType_is_quad_byte_entry(u8 const byte) {
    return Cstl_String_UTF8_4_BYTE_ENTRY
        == (byte & ~(u8) Cstl_String_UTF8_4_BYTE_MASK & 255);
}

Bool Cstl_Utf8ByteType_is_tail_byte(u8 const byte) {
    return Cstl_String_UTF8_TAIL_BYTE_ENTRY
        == (byte & ~(u8) Cstl_String_UTF8_TAIL_BYTE_MASK & 255);
}

Bool Cstl_Utf8ByteType_is_invalid_byte(u8 const byte) {
    return !Cstl_Utf8ByteType_is_single_byte(byte)
        && !Cstl_Utf8ByteType_is_pair_byte_entry(byte)
        && !Cstl_Utf8ByteType_is_triple_byte_entry(byte)
        && !Cstl_Utf8ByteType_is_quad_byte_entry(byte)
        && !Cstl_Utf8ByteType_is_tail_byte(byte);
}

Cstl_Utf8ByteType Cstl_Utf8ByteType_of(u8 const byte) {
    if (Cstl_Utf8ByteType_is_single_byte(byte)) {
        return Cstl_Utf8ByteType_SingleByte;
    }

    if (Cstl_Utf8ByteType_is_pair_byte_entry(byte)) {
        return Cstl_Utf8ByteType_PairByteEntry;
    }

    if (Cstl_Utf8ByteType_is_tail_byte(byte)) {
        return Cstl_Utf8ByteType_TailByte;
    }

    if (Cstl_Utf8ByteType_is_triple_byte_entry(byte)) {
        return Cstl_Utf8ByteType_TripleByteEntry;
    }
    
    if (Cstl_Utf8ByteType_is_quad_byte_entry(byte)) {
        return Cstl_Utf8ByteType_QuadByteEntry;
    }

    return Cstl_Utf8ByteType_InvalidByte;
}

usize Cstl_Utf8ByteType_size(Cstl_Utf8ByteType const self) {
    switch (self) {
    case Cstl_Utf8ByteType_SingleByte:
        return 1;
    case Cstl_Utf8ByteType_PairByteEntry:
        return 2;
    case Cstl_Utf8ByteType_TripleByteEntry:
        return 3;
    case Cstl_Utf8ByteType_QuadByteEntry:
        return 4;

    case Cstl_Utf8ByteType_TailByte:
    case Cstl_Utf8ByteType_InvalidByte:
        // fallthrough
    default:
        Cstl_deny_fmt(
            "invalid enum Cstl_Utf8ByteType value {u32}", (u32) self
        );
    }
}



u32 Cstl_Char_encode_utf8(Cstl_Char const self) {
    // 1 byte encode
    if (self < 0x80) {
        return self;
    }

    // 2 bytes encode
    if (self < 0x800) {
        Cstl_Char mut result = 0;

        result |= Cstl_String_UTF8_2_BYTE_ENTRY << 24;
        result |= Cstl_String_UTF8_TAIL_BYTE_MASK << 16;

        result |= (self & (u32) 0x7C0) << (2 + 16);
        result |= (self & (u32) 0x3F) << 16;

        return result;
    }

    // 3 bytes encode
    if (self < 0x10000) {
        Cstl_Char mut result = 0;

        result |= Cstl_String_UTF8_3_BYTE_ENTRY << 24;
        result |= Cstl_String_UTF8_TAIL_BYTE_ENTRY << 16;
        result |= Cstl_String_UTF8_TAIL_BYTE_ENTRY << 8;

        result |= (self & (u32) 0xF000) << (4 + 8);
        result |= (self & (u32) 0xFC0) << (2 + 8);
        result |= (self & (u32) 0x3F) << 8;

        return result;
    }

    // 4 bytes encode
    if (self < 0x200000) {
        Cstl_Char mut result = 0;

        result |= Cstl_String_UTF8_4_BYTE_ENTRY << 24;
        result |= Cstl_String_UTF8_TAIL_BYTE_ENTRY << 16;
        result |= Cstl_String_UTF8_TAIL_BYTE_ENTRY << 8;
        result |= Cstl_String_UTF8_TAIL_BYTE_ENTRY << 0;

        result |= (self & (u32) 0x1C0000) << 6;
        result |= (self & (u32) 0x3F000) << 4;
        result |= (self & (u32) 0xFC0) << 2;
        result |= self & (u32) 0x3F;

        return result;
    }

    Cstl_deny_fmt("too large character {u32}", (u32) self);
}

Cstl_Char Cstl_Char_from_code(u32 const code) {
    switch (Cstl_Utf8ByteType_of((u8) (code >> 24))) {
    case Cstl_Utf8ByteType_SingleByte:
        return code >> 24;

    case Cstl_Utf8ByteType_PairByteEntry:
        return (0x7C0 & (code >> 18)) | (0x3F & (code >> 16));

    case Cstl_Utf8ByteType_TripleByteEntry:
        return (0xF000 & (code >> 12))
             | (0xFC0 & (code >> 10)) | (0x3F & (code >> 8));

    case Cstl_Utf8ByteType_QuadByteEntry:
        return (0x1C0000 & (code >> 6)) | (0x3F000 & (code >> 4))
             | (0xFC0 & (code >> 2)) | (0x3F & (code >> 0));

    case Cstl_Utf8ByteType_TailByte:
    case Cstl_Utf8ByteType_InvalidByte:
        // fallthrough
    default:
        Cstl_deny_fmt("invalid utf-8 code format {u32}", code);
    }

    return U'\0';
}



void Cstl_set_utf8_output_encoding(void) {
#   if defined(_WIN32) && defined(_WIN64)
        Cstl_assert_fmt(
            SetConsoleOutputCP(65001),
            "SetConsoleOutputCP(65001) failed, error code: {u32}",
            (u32) GetLastError()
        );
#   endif
}



usize CStr_len(CStr const self) {
    CStr mut cur_ptr = self;

    while ('\0' != *(cur_ptr++));

    return (usize) (cur_ptr - self);
}



usize CStrMut_len(CStrMut const self) {
    return CStr_len(self);
}



Cstl_String Cstl_String_with_capacity(usize const cap) {
    return (Cstl_String) {
        .cap = cap,
        .len = 0,
        .ptr = Cstl_mem_alloc_unaligned(sizeof(u8) * cap)
    };
}

Cstl_String Cstl_String_new(void) {
    return Cstl_String_DEFAULT;
}

Cstl_String Cstl_String_from_str(Cstl_str const string) {
    Cstl_String const result = {
        .ptr = Cstl_mem_alloc_unaligned(string.len),
        .len = string.len,
        .cap = string.len
    };

    Cstl_mem_copy(result.ptr, string.ptr, string.len);

    return result;
}

Cstl_String Cstl_String_clone(Cstl_String const* const self) {
    Cstl_String mut result = Cstl_String_with_capacity(self->len);
    result.len = self->len;

    Cstl_mem_copy(result.ptr, self->ptr, result.len);

    return result;
}

void Cstl_String_clone_from(
    Cstl_String mut* const self, Cstl_String const* const src
) {
    if (self->cap < src->len) {
        Cstl_String_free(self);
        *self = Cstl_String_with_capacity(src->len);
    }

    Cstl_mem_copy(self->ptr, src->ptr, src->len);
    self->len = src->len;
}

void Cstl_String_free(Cstl_String const* const self) {
    Cstl_mem_free_unaligned(self->ptr);
    *(Cstl_String mut*) self = Cstl_String_DEFAULT;
}

void Cstl__internal_String_alloc(Cstl_String mut* const self, usize const cap) {
    if (null_mut == self->ptr) {
        *self = Cstl_String_with_capacity(cap);
        return;
    }

    if (self->cap >= cap) {
        return;
    }

    self->ptr = Cstl_mem_realloc_unaligned(self->ptr, sizeof(*self->ptr) * cap);
    self->cap = cap;
}

usize Cstl__internal_String_next_capacity(usize const cur_cap) {
    if (cur_cap < 2) {
        return 4;
    } else {
        return 3 * (cur_cap / 2);
    }
}

void Cstl_String_push(Cstl_String mut* const self, Cstl_Char const symbol) {
    u8 mut bytes[4];
    *(u32*) bytes = Cstl_Char_encode_utf8(symbol);

    usize mut n_bytes = 0;
    while (0 != bytes[n_bytes++] && n_bytes < 4);

    if (self->len + n_bytes > self->cap) {
        Cstl_String_reserve(self, n_bytes);
    }

    for (usize mut i = 0; i < n_bytes; ++i) {
        self->ptr[self->len + i] = bytes[n_bytes - i - 1];
    }

    self->len += n_bytes;
}

void Cstl_String_push_ascii(Cstl_String mut* const self, char const symbol) {
    Cstl_String_push_byte(self, symbol & 127);
}

void Cstl_String_push_byte(Cstl_String mut* const self, u8 const value) {
    if (self->len == self->cap) {
        usize const next_cap = Cstl__internal_String_next_capacity(self->cap);
        Cstl__internal_String_alloc(self, next_cap);
    }

    self->ptr[self->len] = value;
    self->len += 1;
}

Cstl_Char Cstl_String_pop(Cstl_String mut* const self) {
    Cstl_assert(0 != self->len);

    if (Cstl_Utf8ByteType_is_single_byte(self->ptr[self->len - 1])) {
        return Cstl_Char_from_code(self->ptr[--self->len]);
    }
    
    usize mut n_tail_bytes = 1;

    while (Cstl_Utf8ByteType_is_tail_byte(
        self->ptr[self->len - n_tail_bytes - 1]
    )) {
        n_tail_bytes += 1;
    }

    u8 mut bytes[4] = {};

    for (usize mut i = 0; i < sizeof(bytes); ++i) {
        bytes[3 - i] = self->ptr[self->len - n_tail_bytes - 1 + i];
    }

    self->len -= n_tail_bytes + 1;

    return Cstl_Char_from_code(*(u32 const*) bytes);
}

void Cstl_String_shrink_to(Cstl_String mut* const self, usize const cap) {
    if (self->cap <= cap) {
        return;
    }

    self->ptr = Cstl_mem_realloc_unaligned(self->ptr, cap);
    self->cap = cap;
}

void Cstl_String_shrink_to_fit(Cstl_String mut* const self) {
    Cstl_String_shrink_to(self, self->len);
}

void Cstl_String_append(
    Cstl_String mut* const self, Cstl_str const src
) {
    Cstl_String_append_bytes(self, Cstl_str_as_bytes(src));
}

void Cstl_String_append_bytes(
    Cstl_String mut* const self, Cstl_Slice_u8 const slice
) {
    if (self->cap < slice.len + self->len) {
        Cstl_String_reserve(self, slice.len + self->len - self->cap);
    }

    Cstl_mem_copy(self->ptr + self->len, slice.ptr, slice.len);

    self->len += slice.len;
}

void Cstl_String_reserve_exact(
    Cstl_String mut* const self, usize const additional_cap
) {
    Cstl__internal_String_alloc(self, self->cap + additional_cap);
}

void Cstl_String_reserve(
    Cstl_String mut* const self, usize const additional_cap
) {
    usize const next_cap = Cstl__internal_String_next_capacity(self->cap);
    Cstl__internal_String_alloc(
        self,
        usize_max(next_cap, self->cap + additional_cap)
    );
}

Cstl_String Cstl_String_from_raw_c_string(
    char mut* const str, usize const len
) {
    Cstl_String mut result = Cstl_String_with_capacity(len);
    result.len = len;

    for (usize mut i = 0; i < result.len; ++i) {
        result.ptr[i] = str[i] & 127;
    }

    return result;
}

Cstl_String Cstl_String_from_utf8_unchecked(
    u8 mut* const bytes, usize const n_bytes
) {
    Cstl_String mut result = Cstl_String_with_capacity(n_bytes);
    result.len = n_bytes;

    Cstl_mem_copy(result.ptr, bytes, n_bytes);

    return result;
}

Cstl_String Cstl_String_from_utf8(u8 mut* const bytes, usize const n_bytes) {
    Cstl_String mut result = Cstl_String_with_capacity(n_bytes);
    result.len = n_bytes;

    for (usize mut i = 0; i < n_bytes; ) {
        u8 const leader_byte = bytes[i];

        Cstl_Utf8ByteType const class
            = Cstl_Utf8ByteType_of(leader_byte);

        switch (class) {
            case Cstl_Utf8ByteType_SingleByte: {
                result.ptr[i] = leader_byte;
                i += 1;
            } break;

            case Cstl_Utf8ByteType_PairByteEntry: {
                Cstl_assert_fmt(
                    i + 1 < n_bytes,
                    "not enough bytes, `n_bytes = {usize}`",
                    n_bytes
                );

                Cstl_assert_fmt(
                    Cstl_Utf8ByteType_is_tail_byte(bytes[i + 1]),
                    "invalid UTF-8 pair tail byte {u8}",
                    bytes[i + 1]
                );

                result.ptr[i] = leader_byte;
                result.ptr[i + 1] = bytes[i + 1];

                i += 2;
            } break;

            case Cstl_Utf8ByteType_TripleByteEntry: {
                Cstl_assert_fmt(
                    i + 2 < n_bytes,
                    "not enough bytes, `n_bytes = {usize}`",
                    n_bytes
                );

                result.ptr[i] = leader_byte;

                for (usize mut j = 1; j < 3; ++j) {
                    u8 const byte = bytes[i + j];

                    Cstl_assert_fmt(
                        Cstl_Utf8ByteType_is_tail_byte(byte),
                        "invalid UTF-8 triple tail byte {u8}",
                        byte
                    );

                    result.ptr[i + j] = byte;
                }

                i += 3;
            } break;

            case Cstl_Utf8ByteType_QuadByteEntry: {
                Cstl_assert_fmt(
                    i + 3 < n_bytes,
                    "not enough bytes, `n_bytes = {usize}`",
                    n_bytes
                );

                result.ptr[i] = leader_byte;

                for (usize mut j = 1; j < 4; ++j) {
                    u8 const byte = bytes[i + j];

                    Cstl_assert_fmt(
                        Cstl_Utf8ByteType_is_tail_byte(byte),
                        "invalid UTF-8 quad tail byte {u8}",
                        byte
                    );

                    result.ptr[i + j] = byte;
                }

                i += 4;
            } break;

            case Cstl_Utf8ByteType_TailByte:
                // fallthrough
            case Cstl_Utf8ByteType_InvalidByte: {
                Cstl_deny_fmt(
                    "invalid UTF-8 leader byte {u8}", leader_byte
                );
            } break;

            default: {
                Cstl_deny("invalid enum value");
            } break;
        }
    }

    return result;
}

Cstl_str Cstl_String_as_str(Cstl_String const* const self) {
    return *(Cstl_str mut*) self;
}

Cstl_str Cstl_str_split_one(Cstl_str mut* const self, Cstl_str const delim) {
    Cstl_str mut result = { .ptr = self->ptr, .len = 0 };

    while (0 < self->len) {
        if (delim.len <= self->len) {
            Cstl_str const check_str = {
                .ptr = self->ptr,
                .len = delim.len
            };

            if (Cstl_str_eq(check_str, delim)) {
                self->len -= delim.len;
                self->ptr += delim.len;
                break;
            }
        }

        self->len -= 1;
        self->ptr += 1;

        result.len += 1;
    }

    return result;
}

void Cstl_str_print(Cstl_str const self) {
    for (usize mut i = 0; i < self.len; ++i) {
        putchar(self.ptr[i]);
    }
}



Bool Cstl_String_eq(Addr const lhs, Addr const rhs) {
    Cstl_String const* const self = lhs;
    Cstl_String const* const value = rhs;

    if (self->len != value->len) {
        return False;
    }

    for (usize mut i = 0; i < self->len; ++i) {
        if (self->ptr[i] != value->ptr[i]) {
            return False;
        }
    }

    return True;
}

Bool Cstl_String_ne(Addr const lhs, Addr const rhs) {
    return !Cstl_String_eq(lhs, rhs);
}

Cstl_String Cstl_String_concat(usize const n_strings, ...) {
    VariadicArgs mut args;
    VariadicArgs_start(args, n_strings);

    VariadicArgs mut args_clone;
    VariadicArgs_clone_from(args_clone, args);

    usize mut len_sum = 0;

    for (usize mut i = 0; i < n_strings; ++i) {
        len_sum += VariadicArgs_get(args_clone, Cstl_str).len;
    }

    Cstl_String mut result = Cstl_String_with_capacity(len_sum);

    for (usize mut i = 0; i < n_strings; ++i) {
        Cstl_str const value = VariadicArgs_get(args, Cstl_str);

        Cstl_mem_copy(result.ptr + result.len, value.ptr, value.len);
        result.len += value.len;
    }

    VariadicArgs_end(args_clone);
    VariadicArgs_end(args);

    return result;
}

Cstl_Slice_u8 Cstl_String_as_bytes(Cstl_String const* const self) {
    return Cstl_str_as_bytes(Cstl_String_as_str(self));
}



Cstl_str Cstl_str_from_utf8_unchecked(u8 mut* const ptr, usize const len) {
    return (Cstl_str) {
        .len = len,
        .ptr = ptr
    };
}

Bool Cstl_str_eq(Cstl_str const self, Cstl_str const value) {
    if (self.len != value.len) {
        return False;
    }

    for (usize mut i = 0; i < self.len; ++i) {
        if (self.ptr[i] != value.ptr[i]) {
            return False;
        }
    }

    return True;
}

Cstl_Slice_u8 Cstl_str_as_bytes(Cstl_str self) {
    union {
        Cstl_Slice_u8 slice;
        Cstl_str string;
    } const result = { .string = self };

    return result.slice;
}

Cstl_Split Cstl_str_split(Cstl_str const self, Cstl_str const delim) {
    return (Cstl_Split) {
        .delim = delim,
        .string = self
    };
}

Cstl_str Cstl_Split_next(Cstl_Split mut* const self) {
    if (0 == self->string.len) {
        return Cstl_str_DEFAULT;
    }

    Cstl_str mut result = { .ptr = self->string.ptr, .len = 0 };

    while (0 < self->string.len) {
        if (self->delim.len <= self->string.len) {
            Cstl_str const check_str = {
                .ptr = self->string.ptr,
                .len = self->delim.len
            };

            if (Cstl_str_eq(check_str, self->delim)) {
                self->string.len -= check_str.len;
                self->string.ptr += check_str.len;
                break;
            }
        }

        self->string.ptr += 1;
        self->string.len -= 1;

        result.len += 1;
    }

    return result;
}

Bool Cstl_Split_is_expired(Cstl_str const* const ret) {
    return null_mut == ret->ptr;
}



Cstl_SplitAny Cstl_str_split_any(Cstl_str const self, Cstl_Slice const delims) {
    return (Cstl_SplitAny) {
        .delims = delims,
        .string = self
    };
}

Cstl_str Cstl_SplitAny_next(Cstl_SplitAny mut* const self) {
    if (0 == self->string.len) {
        return Cstl_str_DEFAULT;
    }

    Cstl_str mut result = { .ptr = self->string.ptr, .len = 0 };

    while (0 < self->string.len) {
        for (usize mut i = 0; i < self->delims.len; ++i) {
            Cstl_str const delim
                = Cstl_Slice_get_value(self->delims, Cstl_str, i);

            if (delim.len <= self->string.len) {
                Cstl_str const check_str = {
                    .ptr = self->string.ptr,
                    .len = delim.len
                };

                if (Cstl_str_eq(check_str, delim)) {
                    self->string.len -= check_str.len;
                    self->string.ptr += check_str.len;

                    goto break_outer;
                }
            }
        }

        self->string.ptr += 1;
        self->string.len -= 1;

        result.len += 1;
    } break_outer:

    return result;
}

Bool Cstl_SplitAny_is_expired(Cstl_str const* const ret) {
    return null_mut == ret->ptr;
}



Cstl_SplitWhitespace Cstl_str_split_whitespace(Cstl_str const self) {
    return (Cstl_SplitWhitespace) {
        .string = self
    };
}

Cstl_str Cstl_SplitWhitespace_next(Cstl_SplitWhitespace mut* const self) {
    Cstl_str mut result = { .ptr = self->string.ptr, .len = 0 };

    while (0 < self->string.len) {
        if (Cstl_char_is_whitespace(*self->string.ptr)) {
            self->string.ptr += 1;
            self->string.len -= 1;
            break;
        }

        self->string.ptr += 1;
        self->string.len -= 1;

        result.len += 1;
    }

    return result;
}

Bool Cstl_SplitWhitespace_is_expired(Cstl_str const* const ret) {
    return null_mut == ret->ptr;
}



Cstl_Chars Cstl_str_chars(Cstl_str const self) {
    return (Cstl_Chars) {
        .string = self,
    };
}

Cstl_Char Cstl_Chars_next(Cstl_Chars mut* const self) {
    if (0 == self->string.len) {
        return ~(Cstl_Char) 0;
    }

    Cstl_Utf8ByteType const class
        = Cstl_Utf8ByteType_of(*self->string.ptr);

    usize const size = Cstl_Utf8ByteType_size(class);

    self->string.len -= size;
    self->string.ptr += size;

    u32 mut code
        = ((u32) self->string.ptr[0] << 24)
        | ((u32) self->string.ptr[1] << 16)
        | ((u32) self->string.ptr[2] << 8)
        | ((u32) self->string.ptr[3] << 0);

    code &= (~(u32) 0) << (32 - 8 * size);

    return Cstl_Char_from_code(code);
}

Bool Cstl_Chars_is_expired(Cstl_Char const* const ret) {
    return ~(Cstl_Char) 0 == *ret;
}



void Cstl_str_debug(Cstl_str const self) {
    Cstl_print("len = {usize}, value = {str}", self.len, self);
}



Bool Cstl_char_is_ascii_letter(char const self) {
    return ('a' <= self && self <= 'z') || ('A' <= self && self <= 'Z');
}

Bool Cstl_char_is_numeric_letter(char const self) {
    return '0' <= self && self <= '9';
}

Bool Cstl_char_is_whitespace(char const self) {
    return (32 == self) || (9 <= self && self <= 13);
}