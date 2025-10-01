

#ifndef __RANDOM_INTERVAL_H_
#define __RANDOM_INTERVAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <bitstr.h>


typedef struct IntArray {
    int* numbers;
    size_t size;
} int_arr_t;

int_arr_t* __init_int_array( const size_t nsize );


typedef struct Interval {
    int_arr_t* interval;
    int from;
    int to;
    int chosen;
} interval_t;

unsigned int _n_numbers( const int nnumber );
unsigned int _n_numbers_interval( const int nnumber_from, const int nnumber_to );

interval_t* __init_interval( const int nfrom, const int nto );
interval_t* __init_interval_defaultsize( const size_t nsize, const int nfrom, const int nto );
void __init_int_array_interval( const int nfrom, const int nto, struct IntArray* intarr );

int interval_choseone( struct Interval* interv );

// My las vegas algorithm finds a position/number inside of an interval with my min/max quartil theorem.
double _probability_get_las_vegas_sum_to( const double lower_border_start, const double upper_border );
double _probability_get_las_vegas_limes_of( const double lower_border_start, const double upper_border );
struct Interval* probability_interval_choseone( struct Interval* interv );

#ifdef __cplusplus
}
#endif

#endif // __RANDOM_INTERVAL_H_