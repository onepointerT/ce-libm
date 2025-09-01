
#include "integral.h"

#include "functions.h"


double analysis_integral_calculate( const struct Integral* integral ) {
    const double fx_up = analysis_calculate_function_y( integral->function, integral->limit_upper );
    const double fx_low = analysis_calculate_function_y( integral->function, integral->limit_lower );

    const double median_x = ( integral->limit_upper + integral->limit_lower ) / 2;
    const double cx = analysis_calculate_function_y( integral->constant, median_x );
    
    return fx_up - fx_low + cx;
}
