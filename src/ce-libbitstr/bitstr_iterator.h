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


typedef struct BitStrIteratorPair {
    struct BitStrIterator* first;
    struct BitStrIterator* second;
} bitstr_iterator_pair_t;

struct BitStrIteratorPair* __init_bitstr_iterator_pair( struct BitStr* lhs, struct BitStr* rhs );
bool decrement( struct BitStrIteratorPair* it );
bool increment( struct BitStrIteratorPair* it );


typedef struct BitStrIteratorEquation {
    struct BitStrIteratorPair* it;
    struct BitStrIterator* result;
} bitstr_iterator_equation_t;

struct BitStrIteratorEquation* __init_bitstr_iterator_equation( struct BitStr* lhs, struct BitStr* rhs, const unsigned int result_start );
bool decrement_equation( struct BitStrIteratorEquation* it );
bool increment_equation( struct BitStrIteratorEquation* it );

#ifdef __cplusplus
}
#endif
