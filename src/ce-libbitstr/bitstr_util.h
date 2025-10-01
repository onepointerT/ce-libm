#pragma once



#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>


size_t lengthOfInt( const unsigned int bitstr );

unsigned int pushpos( const unsigned int bitstr, const size_t pos, const unsigned short value );
unsigned int getpos( const unsigned int bitstr, const size_t pos );

unsigned int* fillBitStr( const size_t length, const unsigned short value );


typedef struct BitStrPos {
    unsigned int native_pos;
    unsigned short native_value;
    unsigned short new_value;
} bitstr_pos_t;

struct BitStrPos* __init_bitstr_pos( const unsigned int pos_factor_native );
struct BitStrPos* bitstr_pos_get( const unsigned int bitstr, const unsigned int pos );

struct BitStr;
void bitstr_pos_apply_int( const struct BitStrPos* pos, unsigned int* bitstr );
void bitstr_pos_apply( const struct BitStrPos* pos, struct BitStr* bitstr );



#ifdef __cplusplus
}
#endif
