
#include "libm.h"


double multiplinv( const double x ) {

    return nextafter( x, remainder( x, ftaylorCompleteY(x) ) );

}
