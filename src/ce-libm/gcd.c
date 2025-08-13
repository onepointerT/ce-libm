
#include "libm.h"


double gcd( const double a, const double m ) {

    double x = remainder( x, fmod(x, m) );
    double mod =  fmod(x, m);
    while ( mod != 0 ) { 
        mod = fmod( x, m );
        x = remainder( x, mod );
    }

    return x;
}
