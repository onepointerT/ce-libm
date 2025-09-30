
#include "bitstr_type.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libm.h>


bool is_bit( const struct BitStr* bitstr, const size_t pos ) {
    return pos < bitstr->size ? (unsigned int) bitchar(bitstr)[pos] <= (unsigned int) '1' : false;
}


bool is_bit_hex( const struct BitStr* bitstr, const size_t pos ) {
    return pos < bitstr->size ? (unsigned int) bitchar_hex(bitstr)[pos] <= (unsigned int) 0xf : false;
}


unsigned int hex( const struct BitStr* bitstr ) {
    return bitint_hex( bitstr );
}


unsigned int bitint( const struct BitStr* bitstr ) {
    return bitstr->bstr;
}


unsigned int bitint_hex( const struct BitStr* bitstr ) {
    unsigned int* result = (unsigned int*) malloc(sizeof(unsigned int)*bitstr->size/16);
    *result = 0x0;

    sprintf(result, "%x", bitstr->bstr);

    return *result;
}


const char* bitchar( const struct BitStr* bitstr ) {
    char* result = (char*) malloc(sizeof(char)*bitstr->size);
    
    sprintf(result, "%s", bitstr->bstr);

    return result;
}


const char* bitchar_hex( const struct BitStr* bitstr ) {
    char* result = (char*) malloc(sizeof(char)*bitstr->size/16);
    
    sprintf(result, "%x", bitstr->bstr);

    return result;
}


unsigned int bitcharToInt( const char* bitstr ) {
    size_t bitstr_length = sizeof(bitstr) / sizeof(const char*);

    unsigned int result = 0x0;
    bool is_hex = false;
    for ( unsigned int pos = bitstr_length - 1; pos >= 0; pos-- ) {
        if ( ! is_bit(bitstr, pos) && is_bit_hex(bitstr, pos) ) is_hex = true;
        else if ( ! is_bit_hex(bitstr, pos) ) return 0;

        const unsigned int powpos = pow(10, pos+1);
        result = result + 0x0 + (powpos * (unsigned short) bitstr[pos]);
    }

    return result;
}



struct BitStr* __init_bitstr( const size_t bits ) {
    struct BitStr* bitstr = (struct BitStr*) malloc(sizeof(struct BitStr));

    bitstr->size = bits;
    char* bs = (char*) malloc(sizeof(char)*bits);
    for ( unsigned int i = 0; i < bits; i++ ) {
        bs[i] = '0';
    }

    bitstr->bstr = bitcharToInt( bs );

    return bitstr;
}


struct BitStr* __init_bitstr_valued( const size_t bits, const char* value ) {
    struct BitStr* bitstr = __init_bitstr( bits );

    bitstr->bstr = bitcharToInt( value );

    return bitstr;
}