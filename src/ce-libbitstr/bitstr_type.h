#pragma once



#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>


typedef struct BitStr {
    unsigned int bstr;
    size_t size;
} bitstr_t;

bool is_bit( const struct BitStr* bitstr, const size_t pos );
bool is_bit_hex( const struct BitStr* bitstr, const size_t pos );

unsigned int hex( const struct BitStr* bitstr );
unsigned int bitint( const struct BitStr* bitstr );
unsigned int bitint_hex( const struct BitStr* bitstr );
const char* bitchar( const struct BitStr* bitstr );
const char* bitchar_hex( const struct BitStr* bitstr );
unsigned int bitcharToInt( const char* bitstr );

struct BitStr* __init_bitstr( const size_t bits );
struct BitStr* __init_bitstr_valued( const size_t bits, const char* value );


#include "_bitstr_operator.h"


#ifdef __cplusplus
}
#endif
