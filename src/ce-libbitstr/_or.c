
#include "bitstr.h"
#include "_bitstr_operator.h"


struct BitStr* or( const struct BitStr* lhs, const struct BitStr* rhs ) {
    bitstr_iterator_equation_t* eq = __init_bitstr_iterator_equation( lhs, rhs, 0x0 );

    while ( eq->it->first->value != 'x' && eq->it->second->value != 'x' ) {
        eq->result->native->new_value = eq->it->first->native->native_value || eq->it->second->native->native_value;
        bitstr_pos_apply( eq->result->native, eq->result->obj );
    }

    return eq->result->obj;
}
