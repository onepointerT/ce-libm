

#ifndef __PRIM_NUMBER_H_
#define __PRIM_NUMBER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>


int prim_number_gandhi( const unsigned int n );
int prim_number_willians( const unsigned int n );

struct Interval;
struct Interval* prim_number_interval( const unsigned int from_n, const unsigned int to_n );
int* prim_number_array( const unsigned int from_n, const unsigned int to_n );

bool is_prim_number( const int p );

double _barkley_rosser_median( const int pi_n ); // pi(n) # https://de.wikipedia.org/wiki/Primzahlsatz#Die_Primzahlfunktion

struct Interval* prim_chooseone_interval( const unsigned int from_n, const unsigned int to_n );
const unsigned int prim_chooseone( const unsigned int from_n, const unsigned int to_n );


#ifdef __cplusplus
}
#endif

#endif // __PRIM_NUMBER_H_