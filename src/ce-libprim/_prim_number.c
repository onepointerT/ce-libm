
#include "_prim_number.h"

#include <libm.h>

#include "_fourier_transformation.h"
#include "_moebius.h"

int prim_number_gandhi( const unsigned int n ) {
    const double fak_n = _ffak_prim_only( n );
    const double sum = moebius( fak_n ) / (pow(2, fak_n) - 1);

    return abs( 1 - (1/ln(2)) * ln(-(1/2)+sum) );
}


int prim_number_willians( const unsigned int n ) {
    const int upper_sum_border = pow( 2, n );

    int sum_result = 0;
    for ( unsigned int k = 1; k <= upper_sum_border; k++ ) {
        if ( ! is_prim_number(k) ) continue;
        else {
            const double fx1 = abs( k / ( 1 + _barkley_rosser_median(k) ) );
            const double fx2 = abs( pow( fx1, (1/k) ) );
            sum_result = sum_result + fx2;
        }
    }

    return 1 + sum_result;
}


int prim_number_interval( const unsigned int from_n, const unsigned int to_n ) {
    
}

bool is_prim_number( const int p ) {
    const int div_one = p % 1;
    const int div_self = _fourier_transformation( p );

    return div_one == 0 && div_self < 1;
}


double _barkley_rosser_median( const int pi_n ) {
    const int lower = pi_n / ( ln(pi_n) - (1/2) );
    const int upper = pi_n / ( ln(pi_n) - (3/2) );

    return (lower + upper) / 2;
}