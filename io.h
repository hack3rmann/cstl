#ifndef _CSTL_IO_H_
#define _CSTL_IO_H_

#include "types.h"
#include "variadic.h"
#include "util.h"



void Cstl_print(StrLit fmt, ...);

void Cstl_println(StrLit fmt, ...);

void Cstl_print_impl(StrLit fmt, VariadicArgs mut* args);



#ifdef USING_NAMESPACE_CSTL

    #define println Cstl_println
    #define print Cstl_print
    #define print_impl Cstl_print_impl

#endif // USING_NAMESPACE_CSTL

#endif // !_CSTL_IO_H_