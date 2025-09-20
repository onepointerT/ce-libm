
#include "_moebius.h"

#include <libm.h>

#include "_prim_number.h"

int moebius( const unsigned int k ) {

    const unsigned int n = _ffak_prim_only( k );
    const unsigned int sqrt_free = (int) sqrt(n) % 4;

    return is_prim_number(n) ? 1 : ( sqrt_free > 0 ? 0 : -1 ); 
}


int _ffak_prim_only( const unsigned int k ) {
    unsigned int result = 1;
    for ( unsigned int p = 1; p <= k; p++ ) {
        if ( ! is_prim_number(p) ) continue;
        else {
            result = result * p;
        }
    }
    return result;
}