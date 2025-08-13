
#include "libm.h"


double lcm( const double a, const double b ) {
    return (abs(a * b)) / (gcd(a, b));
}
