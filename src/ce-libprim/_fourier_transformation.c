
#include "_fourier_transformation.h"

#include "libm.h"

double _fourier_transformation( const double x ) {
    return cos(6 * M_PI * x) * exp(-M_PI * pow(x, 2));
}
