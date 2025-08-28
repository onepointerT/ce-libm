
#include "libm.h"


double ln( const double a ) {
    return log( a ) / log( exp(a) );
}