#define USING_NAMESPACE_CSTL

#include "../prelude.h"
#include "../iterator.h"
#include "../format.h"
#include "../memory.h"



typedef struct Countdown {
    u32 value;
    u32 (*next)(struct Countdown mut*);
    bool (*is_expired)(struct Countdown const*);
} Countdown;

u32 Countdown_next(Countdown mut* const self) {
    if (u32_MAX == self->value) {
        return u32_MAX;
    }

    return self->value--;
}

bool Countdown_is_expired(Countdown const* const self) {
    return u32_MAX == self->value;
}

Countdown Countdown_new(u32 const start) {
    return (Countdown) {
        .value = start,
        .is_expired = Countdown_is_expired,
        .next = Countdown_next
    };
}



i32 main(void) {
    Countdown mut iter = Countdown_new(10);

    while (!iter.is_expired(&iter)) {
        println("{u32}", iter.next(&mut iter));
    }

    return EXIT_SUCCESS;
}