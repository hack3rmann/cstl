#ifndef _CSTL_IO_H_
#define _CSTL_IO_H_

#include "types.h"
#include "variadic.h"
#include "util.h"



#define Cstl_println(fmt, args...) \
    Cstl_print(fmt "\n", args)



void Cstl_print(StrLit fmt, ...);

void Cstl_print_impl(StrLit fmt, VariadicArgs mut* args);



#ifdef USING_NAMESPACE_CSTL

    #define println(fmt, args...) \
        print(fmt "\n", args)

    void print(StrLit fmt, ...);

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_IO_H_