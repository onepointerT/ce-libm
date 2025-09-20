
#include "libm.h"


double ffak( const double n ) {
    double result = n;
    for ( unsigned int c = 1; c <= n; c++ ) {
        result = result * c;
    }
    return result;
}