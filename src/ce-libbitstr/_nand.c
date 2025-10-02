
#include "bitstr.h"
#include "_bitstr_operator.h"


struct BitStr* nand( const struct BitStr* lhs, const struct BitStr* rhs ) {
    bitstr_iterator_equation_t* eq = __init_bitstr_iterator_equation( lhs, rhs, 0x0 );

    while ( eq->it->first->value != 'x' && eq->it->second->value != 'x' ) {
        eq->result->native->new_value = 0x0 + (unsigned short) ( eq->it->first->native->native_value != 0x1 || eq->it->second->native->native_value != 0x1 );
        bitstr_pos_apply( eq->result->native, eq->result->obj );
    }

    return eq->result->obj;
}
