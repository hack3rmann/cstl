# `cstl` the `C` standard library

## Abstract

`cstl` (pronounced "si es ti el") is yet another standard library for `C` programming language featuring readable and convenient API.

## Overview

### Features

- Almost all library names are prefixed with `Cstl_`. It can be opt-out with `#define USING_NAMESPACE_CSTL`
- UTF-8-encoded strings in two variants: `String` and `str`. `String`s are heap-allocated and `str`s are just string views
- Any type dynamic arrays: `Vec` and `Vec_Type`s (where `Type` is any basic type)
- `Slice` array views and algorithms on them
- Overridden basic types: `u8`, `i8`, `u16`, `i16`, `u32`, `i32`, `u64`, `i64`, `Bool`, `usize`, `isize`, `Addr`, `AddrMut`, `StrLit`, `CStr`, `CStrMut`.
- Any mutable data explicitly marked with empty macro `mut` for convenience.
- Formatted console I/O
- Type formatters and support for user-defined type formatters
- Basic iterator support (`Split`s, `Chars`es and others)
- Helpful macros for error handling: `deny_fmt`, `assert_fmt`, `todo` and `unreachable`.

### `String` example

String concatenation. Defined in `<cstl/string.h>`

```c
Cstl_init();

str const first = str("Hello");
String const second = String("World😊");

String const result = String_concat(3, first, str(", "), *(str const*) &second);

// outputs: Hello, World😊
println("{String}", &result);

String_free(&result);
String_free(&second);
```

`str` (can be obtained by defining `USING_NAMESPACE_CSTL`) macro here creates `str` from `C` string literal. `String_concat` allocates new `Striing` and concatenates `first`, `str(", ")` and second to `result`. `String` is heap-allocated so the caller should also call `String_free` after using this string. It is safe to cast `String` to `str`, because `String` inherits structure from `str`. `Cstl_init` sets output encoding to UTF-8.

### `Vec` example

`Vec` expansion while adding elements to it.

```c
// allocate vector with capacity of 4 elements
Vec mut vec = Vec_with_capacity(4, sizeof(i32));

Vec_push_value(&mut vec, i32, 1);
Vec_push_value(&mut vec, i32, 2);
Vec_push_value(&mut vec, i32, 3);
Vec_push_value(&mut vec, i32, 4);
Vec_push_value(&mut vec, i32, 5);
Vec_push_value(&mut vec, i32, 6);

// outputs: [1, 2, 3, 4, 5, 6]
println("{Vec::i32}", &vec);

Vec_free(&vec);
```

### User-defined formatter example

`cstl` user can define his own formatter for any type.

```c
typedef struct Point {
    i32 x, y;
} Point;

void Point_fmt(String mut* buf, str const fmt, Addr value_ptr) {
    Point const value = *(Point const*) value_ptr;

    String_push_ascii(buf, '(');
    i32_fmt(buf, fmt, &value.x);

    String_append(buf, str(", "));
    i32_fmt(buf, fmt, &value.y);

    String_push_ascii(buf, ')');
}

i32 main(void) {
    Point const point = { .x = 64, .y = -42 };

    // outputs: (64, -42)
    println("{*}", Point_fmt, &point);
}
```

### Iterator example

Iterators can help with splitting a string by another string.

```c
Split mut iter = str_split(
    str("String, splitted, with, commas, and, spaces"),
    str(", ")
);

Vec mut substrings = Vec_new(sizeof(str));

iterate(Split, iter, substring, {
    Vec_push_value(&mut substrings, str, substring);
});

// outputs: Stringsplittedwithcommasandspaces
println("{Vec:**:str}", &substrings);

Vec_free(&substrings);
```

### Error handling

```c
AddrMut const ptr = mem_alloc(99999999999999999);

assert_fmt(null_mut != ptr, "ptr = {Addr} is invalid pointer value", ptr);
```

This code fails with message:

```plain_text
deny called in function `main` in file `test.c` on line `13`:
assertion failed: `null_mut != ptr` ptr = 0x0 is invalid pointer value
```
