#include "types.h"



Bool Bool_implies(Bool const self, Bool const other) {
    return self <= other;
}



Addr Addr_from_usize(usize const value) {
    union {
        Addr address;
        usize integer;
    } const result = { .integer = value };

    return result.address;
}

usize Addr_as_usize(Addr const self) {
    union {
        Addr address;
        usize integer;
    } const result = { .address = self };

    return result.integer;
}



AddrMut AddrMut_from_usize(usize const value) {
    union {
        AddrMut address;
        usize integer;
    } const result = { .integer = value };

    return result.address;
}

usize AddrMut_as_usize(AddrMut const self) {
    union {
        AddrMut address;
        usize integer;
    } const result = { .address = self };

    return result.integer;
}