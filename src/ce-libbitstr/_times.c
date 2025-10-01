
#include "bitstr.h"
#include "bitstr_util.h"
#include "_bitstr_operator.h"


struct BitStr* times( const struct BitStr* lhs, const struct BitStr* rhs, bool binary ) {
    bitstr_iterator_equation_t* eq = __init_bitstr_iterator_equation( lhs, rhs, 0x0 );

    unsigned short rest = 0x00;
    unsigned short result = 0x000;
    while ( eq->it->first->value != 'x' && eq->it->second->value != 'x' ) {
        // Find out the summand overflow
        result = eq->it->first->native->native_value / eq->it->second->native->native_value;
        rest = 0x000 + (unsigned short) (result / 10) + rest;

        unsigned short last_bit = getlastbit( result, binary );

        eq->result->native->new_value = last_bit + getlastbit( rest, binary );
        
        if ( eq->result->native->new_value > (binary ? 0x1 : 0xf) ) {
            rest = rest + getlastbitnot( eq->result->native->new_value );
            eq->result->native->new_value = getlastbit( eq->result->native->new_value, binary );
        }
        bitstr_pos_apply( eq->result->native, eq->result->obj );
    }

    return eq->result->obj;
}
