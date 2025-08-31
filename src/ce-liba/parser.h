#pragma once

#include <float.h>


#ifdef __cplusplus
extern "C" {
#endif


struct ArithmeticParser {
    const char* summand;
    const char* reststr;

    struct ArithmeticParser* prev;
    struct ArithmeticParser* next;
};

struct ArithmeticParser* analysis__init_parser( const char* summand, const char* reststr, struct ArithmeticParser* prev );
struct ArithmeticParser* analysis__init_parser_default( const char* funcstr );
struct ArithmeticParser* analysis__get_next_parser( const char* reststr, struct ArithmeticParser* prev );


#ifdef __cplusplus
}
#endif
