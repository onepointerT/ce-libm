#pragma once



#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

struct BitStr;
struct BitStrPos;

typedef struct BitStrIterator {
    size_t pos;
    char value; // '\0' means smallest/most right value reached, 'x' means greatest/most left value reached
    struct BitStr* obj;
    struct BitStrPos* native;
} bitstr_iterator_t;

struct BitStrIterator* __init_bitstr_iterator( const size_t position, struct BitStr* object );

bool _update_bitstr_iterator( struct BitStrIterator* bsi );
bool _update_bitstr_iterator_from_bitstr( struct BitStrIterator* bsi, const struct BitStr* bitstr );
struct BitStrIterator* _update_bitstr_iterator_next( struct BitStrIterator* bsi );
struct BitStrIterator* _update_bitstr_iterator_previous( struct BitStrIterator* bsi );
bool bitstr_iterator_start( const struct BitStrIterator* bsi );
bool bitstr_iterator_end( const struct BitStrIterator* bsi );

bool bitstr_bit_is_bit( const char bit );
bool bitstr_bit_is_bithex( const char bit );


#ifdef __cplusplus
}
#endif
