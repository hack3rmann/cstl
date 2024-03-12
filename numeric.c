#include "numeric.h"





i8 i8_abs(i8 self) {
    usize const tmp = self >> (8 * sizeof(i8) - 1);
    self ^= tmp;
    return self + (tmp & 1);
}

i16 i16_abs(i16 self) {
    usize const tmp = self >> (8 * sizeof(i16) - 1);
    self ^= tmp;
    return self + (tmp & 1);
}

i32 i32_abs(i32 self) {
    usize const tmp = self >> (8 * sizeof(i32) - 1);
    self ^= tmp;
    return self + (tmp & 1);
}

i64 i64_abs(i64 self) {
    usize const tmp = self >> (8 * sizeof(i64) - 1);
    self ^= tmp;
    return self + (tmp & 1);
}

isize isize_abs(isize self) {
    usize const tmp = self >> (8 * sizeof(isize) - 1);
    self ^= tmp;
    return self + (tmp & 1);
}
