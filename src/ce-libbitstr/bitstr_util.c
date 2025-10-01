
#include "bitstr_util.h"

#include <stdlib.h>
#include <string.h>

#include <libm.h>

#include "bitstr_type.h"


size_t lengthOfInt( const unsigned int bitstr ) {
    size_t result = 0;
    unsigned int rest = 1;

    if ( bitstr == 0 || bitstr < 10 ) return 1;

    unsigned int expo = 1;
    while ( (unsigned int) rest > (unsigned int) 0xf && rest > 10 ) {
        rest = bitstr % (unsigned int) pow(10, expo);
        if ( rest == 0 ) return expo;
        else ++expo
    }

    return result + 1;
}


unsigned int pushpos( const unsigned int bitstr, const size_t pos, const unsigned short value ) {
    if ( lengthOfInt(bitstr) <= pos ) return 0x0;

    bitstr_pos_t* bitstr_pos = bitstr_pos_get( bitstr, pos );
    bitstr_pos->new_value = value;

    unsigned int result = bitstr;
    bitstr_pos_apply_int( bitstr_pos, &result );
    
    return result;
}


unsigned int getpos( const unsigned int bitstr, const size_t pos ) {
    return (bitstr % (unsigned int)(pos - 1)) - (bitstr % (unsigned int)(pos - 2));
}


unsigned int* fillBitStr( const size_t length, const unsigned short value ) {
    char* bitstr = (char*) malloc(sizeof(char)*length);

    for ( unsigned int b = length - 1; b >= 0; b-- ) {
        bitstr[b] = (char) value;
    }

    unsigned int* bitint = (unsigned int*) malloc(sizeof(unsigned int));
    *bitint = 0x0;
    sprintf(bitint, "%x", bitstr);

    return bitint;
}


struct BitStrPos* __init_bitstr_pos( const unsigned int pos_factor_native ) {
    struct BitStrPos* pos = (struct BitStrPos*) malloc(sizeof(struct BitStrPos));

    pos->native_pos = pos_factor_native;
    pos->native_value = 0;
    pos->new_value = 0;

    return pos;
}

struct BitStrPos* bitstr_pos_get( const unsigned int bitstr, const unsigned int pos ) {
    struct BitStrPos* bitstr_pos = __init_bitstr_pos( (unsigned int) pow(10, pos) );

    unsigned int rest_int = bitstr % (bitstr_pos->native_pos - 1);
    bitstr_pos->native_value = ((bitstr % bitstr_pos->native_pos) - rest_int) / bitstr_pos->native_pos;

    return bitstr_pos;
}


void bitstr_pos_apply_int( const struct BitStrPos* pos, unsigned int* bitstr ) {
    char* cbit = (char*) malloc(sizeof(bitstr)/sizeof(unsigned int));
    sprintf(cbit, "%s", *bitstr);
    cbit[pos->native_pos-1] = (char) pos->new_value;
    *bitstr = bitcharToInt(cbit);
}


void bitstr_pos_apply( const struct BitStrPos* pos, struct BitStr* bitstr ) {
    bitstr_pos_apply_int( pos, &bitstr->bstr );
}
