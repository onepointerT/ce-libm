
#include "bitstr_iterator.h"

#include <stdlib.h>

#include "bitstr_type.h"
#include "bitstr_util.h"


struct BitStrIterator* __init_bitstr_iterator( const size_t position, struct BitStr* object ) {
    struct BitStrIterator* it = (struct BitStrIterator*) malloc(sizeof(struct BitStrIterator));

    it->obj = object;
    it->pos = position;
    _update_bitstr_iterator( it );

    return it;
}

bool _update_bitstr_iterator( struct BitStrIterator* bsi ) {
    if ( bsi->obj == 0 ) {
        bsi->value = '\0';
        return false;
    } else if ( bsi->pos >= bsi->obj->size ) {
        bsi->value = 'x';
        return false;
    }

    bsi->native = bitstr_pos_get( bsi->obj->bstr, bsi->pos );
    bsi->value = (char) bsi->native->native_value;
    
    return true;
}

bool _update_bitstr_iterator_from_bitstr( struct BitStrIterator* bsi, const struct BitStr* bitstr ) {
    if ( bsi->pos >= bitstr->size ) {
        bsi->value = 'x';
        return false;
    }

    bsi->native = bitstr_pos_get( bitstr->bstr, bsi->pos );
    bsi->value = (char) bsi->native->native_value;

    return true;
}

struct BitStrIterator* _update_bitstr_iterator_next( struct BitStrIterator* bsi ) {
    if ( bsi->obj == 0 ) return 0;
    else if ( bsi->value == '\0' ) { 
        bsi->pos = 0;
        bsi->native = getpos( bsi->obj->bstr, 0 );
        bsi->value = (char) bsi->native->native_value;
        return bsi;
    } else if ( bsi->pos == bsi->obj->size - 1 ) { bsi->value = 'x'; bsi->native = 0; return bsi; }
    else bsi->pos++;

    _update_bitstr_iterator( bsi );
    return bsi;
}

struct BitStrIterator* _update_bitstr_iterator_previous( struct BitStrIterator* bsi ) {
    if ( bsi->obj == 0 ) return 0;
    else if ( bsi->value == 'x' ) {
        bsi->pos = bsi->obj->size - 1;
        bsi->native = getpos( bsi->obj->bstr, bsi->pos );
        bsi->value = (char) bsi->native->native_value;
    } else if ( bsi->pos == 0 ) { bsi->value = '\0'; bsi->native = 0; return bsi; }
    else bsi->pos--;

    _update_bitstr_iterator( bsi );
    return bsi;
}

bool bitstr_iterator_start( const struct BitStrIterator* bsi ) {
    if ( bsi == 0 ) return false;
    return bsi->value == '\0' && bsi->native == 0;
}

bool bitstr_iterator_end( const struct BitStrIterator* bsi ) {
    if ( bsi == 0 ) return false;
    return bsi->value == 'x' && bsi->native == 0;
}


bool bitstr_bit_is_bit( const char bit ) {
    return bit == '0' || bit == '1';
}


bool bitstr_bit_is_bithex( const char bit ) {
    return bitstr_bit_is_bit(bit) || bit == '2' || bit == '3' || bit == '4'
        || bit == '5' || bit == '6' || bit == '7' || bit == '8' || bit == '9'
        || bit == 'a' || bit == 'b' || bit == 'c' || bit == 'd' || bit == 'e'
        || bit == 'f';
}
