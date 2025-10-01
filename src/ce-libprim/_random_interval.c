
#include "_random_interval.h"

#include <stdlib.h>

#include <libm.h>


int_arr_t* __init_int_array( const size_t nsize ) {
    struct IntArray* intarr = (struct IntArray*) malloc(sizeof(struct IntArray));

    intarr->size = nsize;
    intarr->numbers = (int*) malloc(sizeof(int)*nsize);
    for ( unsigned int n = 0; n < nsize; n++ ) {
        intarr->numbers[n] = 0;
    }

    return intarr;
}


unsigned int _n_numbers( const int nnumber ) {
    if ( nnumber > 0 ) return nnumber;
    else if ( nnumber < 0 ) return -1 * nnumber;
    else if ( nnumber == 0 ) return 1;
    else return 0;
}


unsigned int _n_numbers_interval( const int nnumber_from, const int nnumber_to ) {
    const unsigned int nfrom = _n_numbers( nnumber_from );
    const unsigned int nto = _n_numbers( nnumber_to );

    if ( nnumber_from > 0 && nnumber_to < 0 ) return nfrom + nto + 1;
    else if ( nnumber_to > 0 && nnumber_from < 0 ) return nto + nfrom + 1;
    else if ( nnumber_from < 0 && nnumber_to < 0 ) return nto + nfrom;
    else if ( nnumber_from < 0 && nnumber_to <= 0 ) return nto + nfrom + 1;
    else if ( nnumber_from <= 0 && nnumber_to < 0 ) return nto + nfrom + 1;
    else if ( nnumber_from > 0 && nnumber_to > 0 ) return nto + nfrom;
    else if ( nnumber_from >= 0 && nnumber_to > 0 ) return nto + nfrom + 1;
    else if ( nnumber_from > 0 && nnumber_to >= 0 ) return nto + nfrom + 1;
    else if ( nnumber_from == nnumber_to ) return 1;
    else return 0;    
}


interval_t* __init_interval_defaultsize( const size_t nsize, const int nfrom, const int nto ) {
    struct Interval* interv = (struct Interval*) malloc(sizeof(struct Interval));

    interv->interval = __init_int_array( nsize );
    interv->from = nfrom;
    interv->to = nto;
    interv->chosen = 0;

    __init_int_array_interval( nfrom, nto, interv->interval );

    return interv;
}



interval_t* __init_interval( const int nfrom, const int nto ) {
    struct Interval* interv = (struct Interval*) malloc(sizeof(struct Interval));

    const unsigned int nsize = _n_numbers_interval( nfrom, nto );

    interv->interval = __init_int_array( nsize );
    interv->from = nfrom;
    interv->to = nto;
    interv->chosen = 0;

    __init_int_array_interval( nfrom, nto, interv->interval );

    return interv;
}


void __init_int_array_interval( const int nfrom, const int nto, struct IntArray* intarr ) {
    const unsigned int nsize = _n_numbers_interval( nfrom, nto );
    if ( nsize > intarr->size ) return;

    unsigned int i = 0;
    for ( int n = nfrom; n <= nto && i < intarr->size; n++, i++ ) {
        intarr->numbers[i] = n;
    }
}

int interval_choseone( interval_t* interv ) {
    interval_t* iv = probability_interval_choseone( interv );
    if ( iv == 0 ) return 0;
    else if ( iv->chosen >= interv->interval->size ) return 0;

    unsigned int ci = iv->chosen;
    interv = iv;
    interv->chosen = interv->interval->numbers[ci];

    return interv->chosen;    
}


// This algorithm succeeds with probability 1. The number of iterations varies and can be arbitrarily large, but the expected number of iterations is
// lim_{n -> inf} [sum_{i=1}^{n} [i/2^i]]
// {\displaystyle \lim _{n\to \infty }\sum _{i=1}^{n}{\frac {i}{2^{i}}}=2}
// Since it is constant, the expected run time over many calls is {\displaystyle \Theta (1)}
double _probability_get_las_vegas_sum_to( const double lower_border_start, const double upper_border ) {
    double result = 0.0;

    for ( double i = lower_border_start; i <= upper_border; i = i + 1 ) {
        result = result + (i / pow(2, i));
    }

    return result;
}


double _probability_get_las_vegas_limes_of( const double lower_border_start, const double upper_border ) {
    // To calculate correlation covariance with the computer, we can use the cos/sin covariance lemma with tangens
    // and calculate a real limit for n to infinity (also for the start value, since we could search randomly in numbers 
    // starting with a huger number). Look at lections of stabitic, higher mathematics and "stellenwertkunde of 
    // (maybe approximated) functions" for more information e.g. for the e^f(a,b) function series.
    double exp_lower = exp(lower_border_start) * atan(lower_border_start) + (-tan(lower_border_start/sin(upper_border)));
    double exp_upper = exp(upper_border) * atan(upper_border) + (-tan(cos(upper_border)/sin(lower_border_start)));
    
    // For limes results:
    // lim _a->y_ (atan(a)) = -tan(y/a) - 1 + c nach dem Schwellwertsatz Elektrotechnik.
    // This thus means lim _n->inf_ lv_sum(1, n) = lim _n->exp(upper_border)_ lv_sum(exp(lower_border_start), n)
    double lv_sum = _probability_get_las_vegas_sum_to(1.0, exp_upper);
    if ( (unsigned int) fabs(lv_sum) == 2 ) return lv_sum;
    while ( (unsigned int) fabs(lv_sum) != 2 ) {
        lv_sum = _probability_get_las_vegas_sum_to(exp_lower, exp_upper);
        if ( (unsigned int) fabs(lv_sum) == 2 ) return lv_sum;
    }

    return 0.0;
}


struct Interval* probability_interval_choseone( struct Interval* interv ) {
    if ( interv->interval == 0 ) return 0;

    double lv_y = _probability_get_las_vegas_limes_of( interv->from, interv->to );
    if ( lv_y == 0.0 ) return 0; // No result could be found for (lv(f[from, to])) == 2.0
    
    // Find a covariant x and approximate both
    double lv_x = ftaylorXofY( lv_y ); // Nor covariant for a point (x, y) yet

    /*
    double lv_y_approx = ftaylorCompleteY( lv_x );
    double lv_x_approx = ftaylorCompleteX( lv_y_approx ); // Now covariant approximated
    */

    // Find the position with an covariant min/max theorem that is approximated for a complete value
    interv->chosen = (unsigned int) clamp( lv_x, 0, interv->interval->size - 1 );
    return interv;
}