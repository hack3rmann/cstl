#ifndef _CSTL_VARIADIC_H_
#define _CSTL_VARIADIC_H_



#define VariadicArgs_start(self, last_arg) \
    __builtin_va_start(self, last_arg)

#define VariadicArgs_end(self) \
    __builtin_va_end(self)

#define VariadicArgs_get(self, Type) \
    __builtin_va_arg(self, Type)

#define VariadicArgs_clone_from(self, src) \
    __builtin_va_copy(self, src)

typedef __builtin_va_list VariadicArgs;



#endif // !_CSTL_VARIADIC_H_