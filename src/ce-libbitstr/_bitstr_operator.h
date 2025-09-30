#pragma once



#ifdef __cplusplus
extern "C" {
#endif


struct BitStr;


struct BitStr* and( const struct BitStr* lhs, const struct BitStr* rhs );
struct BitStr* or( const struct BitStr* lhs, const struct BitStr* rhs );


struct BitStr* plus( const struct BitStr* lhs, const struct BitStr* rhs );
struct BitStr* minus( const struct BitStr* lhs, const struct BitStr* rhs );
struct BitStr* times( const struct BitStr* lhs, const struct BitStr* rhs );
struct BitStr* divide( const struct BitStr* lhs, const struct BitStr* rhs );


struct BitStr* xor( const struct BitStr* lhs, const struct BitStr* rhs );
struct BitStr* nand( const struct BitStr* lhs, const struct BitStr* rhs );


#include "_bitstr_operator.h"


#ifdef __cplusplus
}
#endif
