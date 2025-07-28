#include "libm.h"


double ftaylor( double x ) {
    double t1 = exp( - 1 / pow( x, 2 ) );
    double t2 = exp( - 1 / 2 * x );
    double t3 = exp( - 1 / 2 );

    double T = t1 + t2 + t3;

    return T;
}

double ftaylorX( double ty ) {
    // Es gilt außerdem y = exp(x) * (1+n+x) <= n=3 => y/4x = exp(x) <=> y/4 = exp(x)*x 
    double x_nearly = ty / 4;
    double x_approx = x_nearly / exp( x_nearly );
    double next_x1 = nextafter( exp(x_approx)*x_approx, x_nearly );
    double next_x2 = remainder( next_x1, ty );
    return nextafter( next_x2, x_nearly );
}

double ftaylorY( double tx ) {
    double T1 = ftaylor( tx );

    double x_approx = nextafter( tx, ftaylorX(T1) );
    double T2 = ftaylor( x_approx );

    return nextafter( T1, T2 ); 
}

double ftaylorPolyX( double tx ) {
    return ftaylorX( ftaylorY(tx) );
}

double ftaylorPolyY( double tx ) {
    return ftaylorY( tx );
}

double ftaylorCompleteX( double x ) {
    return ftaylorPolyX( ftaylor(x) );
}

double ftaylorCompleteY( double x ) {
    return ftaylorPolyY( ftaylor(x) );
}

double ftaylorXofY( double y ) {

    double x_approx = remainder( y / 4, y );
    double x_direction = exp( x_approx ) * x_approx;
    double next_x1 = nextafter( exp(x_approx)*x_direction, x_direction );
    double next_x2 = remainder( next_x1, y );
    double next_x = nextafter( next_x2, x_approx );
    double x_next = nextafter( next_x2, next_x );

    return ftaylorX( x_next );
}

double ftaylorduo( double T1, double T2 ) {
    double x_next = ftaylorCompleteX( remainder( ftaylorXofY(T1), T2 ) );

    return ftaylorY( x_next );
}

double ftaylorXduo( double x ) {
    double T1 = ftaylor( x );

    double x_approx = nextafter( x, ftaylorX(T1) );
    double y = ftaylorduo( T1, ftaylor( x_approx ) );
    return ftaylorXofY( y );
}

double ftaylorYduo( double x ) {
    double T1 = ftaylor( x );
    double y_start = ftaylorY( T1 );

    double x_approx = nextafter( remainder( x, y_start), ftaylorX(T1) );
    return ftaylorduo( T1, ftaylor( x_approx ) );
}
