#pragma once



#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


struct BitStr;


struct BitStr* and( const struct BitStr* lhs, const struct BitStr* rhs );
struct BitStr* or( const struct BitStr* lhs, const struct BitStr* rhs );


struct BitStr* plus( const struct BitStr* lhs, const struct BitStr* rhs, bool binary );
struct BitStr* minus( const struct BitStr* lhs, const struct BitStr* rhs, bool binary );
struct BitStr* times( const struct BitStr* lhs, const struct BitStr* rhs, bool binary );
struct BitStr* divide( const struct BitStr* lhs, const struct BitStr* rhs, bool binary );


struct BitStr* xor( const struct BitStr* lhs, const struct BitStr* rhs );
struct BitStr* nand( const struct BitStr* lhs, const struct BitStr* rhs );


#ifdef __cplusplus
}
#endif
