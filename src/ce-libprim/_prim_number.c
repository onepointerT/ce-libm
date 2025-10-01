
#include "_prim_number.h"

#include <stdlib.h>

#include <libm.h>

#include "_fourier_transformation.h"
#include "_moebius.h"
#include "_random_interval.h"


int prim_number_gandhi( const unsigned int n ) {
    const double fak_n = _ffak_prim_only( n );
    const double sum = moebius( fak_n ) / (pow(2, fak_n) - 1);

    return fabs( 1 - (1/ln(2)) * ln(-(1/2)+sum) );
}


int prim_number_willians( const unsigned int n ) {
    const int upper_sum_border = pow( 2, n );

    int sum_result = 0;
    for ( unsigned int k = 1; k <= upper_sum_border; k++ ) {
        if ( ! is_prim_number(k) ) continue;
        else {
            const double fx1 = fabs( k / ( 1 + _barkley_rosser_median(k) ) );
            const double fx2 = fabs( pow( fx1, (1/k) ) );
            sum_result = sum_result + fx2;
        }
    }

    return 1 + sum_result;
}


int* prim_number_array( const unsigned int from_n, const unsigned int to_n ) {
    const unsigned int n_amount = to_n - from_n;

    int* intArr = (int*) malloc(sizeof(int)*(n_amount)); // Maximal this much numbers

    unsigned int i = 0;
    for ( unsigned int p = prim_number_willians(from_n)
        ; p <= to_n && i < n_amount && is_prim_number(p)
        ; p = prim_number_willians(p), i++
    ) {
        intArr[i] = p;
    }

    int* primArr = (int*) malloc(sizeof(int)*(i+1));
    for ( unsigned int pia = 0; pia < i; pia++ ) {
        primArr[pia] = intArr[pia];
    }

    return primArr;
}


struct Interval* prim_number_interval( const unsigned int from_n, const unsigned int to_n ) {

    int* primArr = prim_number_array( from_n, to_n );
    unsigned int i = sizeof(*primArr) / sizeof(int);

    struct IntArray* prim_arr = __init_int_array( i );
    prim_arr->numbers = primArr;
    struct Interval* prim_interv = __init_interval_defaultsize( i, primArr[0], primArr[i-1] );
    prim_interv->interval = prim_arr;

    return prim_interv;
}

bool is_prim_number( const int p ) {
    const int div_one = p % 1;
    const int div_self = _fourier_transformation( p );

    return div_one == 0 && div_self < 1 && div_self != 0;
}


double _barkley_rosser_median( const int pi_n ) {
    const int lower = pi_n / ( ln(pi_n) - (1/2) );
    const int upper = pi_n / ( ln(pi_n) - (3/2) );

    return (lower + upper) / 2;
}



struct Interval* prim_chooseone_interval( const unsigned int from_n, const unsigned int to_n ) {
    struct Interval* prim_interv = prim_number_interval( from_n, to_n );
    if ( probability_interval_choseone(prim_interv) != 0 ) return prim_interv;
    return 0;
}



const unsigned int prim_chooseone( const unsigned int from_n, const unsigned int to_n ) {
    struct Interval* prim_interv = prim_number_interval( from_n, to_n );
    if ( interval_choseone(prim_interv) == 0 ) return 0;
    return prim_number_gandhi( prim_interv->chosen );
}