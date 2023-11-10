#ifndef _CSTL_ITERATOR_H_
#define _CSTL_ITERATOR_H_



#define Cstl_iterate(IterType, iter, next, tt) \
    { \
        typeof(IterType ## _next(&mut iter)) mut next; \
        \
        while (True) { \
            next = IterType ## _next(&mut iter); \
            \
            if (IterType ## _is_expired(&next)) { \
                break; \
            } \
            \
            tt \
        } \
    }



#ifdef USING_NAMESPACE_CSTL

    #define iterate Cstl_iterate

#endif



#endif // !_CSTL_ITERATOR_H_