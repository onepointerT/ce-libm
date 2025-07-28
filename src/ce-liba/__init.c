
#include "liba.h"
#include "libm.h"



struct Point* analysis__init_point( const double x, const double y ) {
    struct Point* pnt = (struct Point*) malloc(sizeof(struct Point));
    
    pnt->x = x;
    pnt->y = y;

    return pnt;
}

struct Point* analysis__init_point_default() {
    return analysis__init_point( 0.0, 0.0 );
}

struct LinearPath* analysis__init_linear_path( const struct Point* px, struct LinearPath* next, struct LinearPath* prev ) {

    struct LinearPath* lp = (struct LinearPath*) malloc(sizeof(struct LinearPath));

    lp->point = px;
    lp->next = next;
    lp->prev = prev;

    return lp;
}

struct LinearPath* analysis__init_linear_path_default( const struct Point* px ) {
    
    struct LinearPath* lp = (struct LinearPath*) malloc(sizeof(struct LinearPath));

    lp->point = px;
    lp->next = NULL;
    lp->prev = NULL;

    return lp;
}

struct Parabole* analysis__init_parabole( const struct Point* px ) {

    struct Parabole* para = (struct Parabole*) malloc(sizeof(struct Parabole));

    para->start = px;
    para->end = NULL;
    para->ceil = NULL;
    para->floor = NULL;
    para->linear = NULL;
    para->shapes = NULL;

    analysis_init_parabole( para );

    return para;
}

struct Parabole* analysis__init_parabole_from_linearPath( const linear_path_t* linearPath ) {
    struct Parabole* para = (struct Parabole*) malloc(sizeof(struct Parabole));

    linear_path_t* lp = linearPath;
    if ( lp->next != NULL ) {
        while ( lp->next != NULL ) { lp = lp->next; }
        para->end = lp->point;
    } else para->end = analysis__init_point_default();

    analysis_init_parabole( para );

    return para;
}


void analysis_init_parabole( struct Parabole* para ) {

    para->end->y = ftaylor( remainder(ftaylorXofY(para->start->y), para->start->y) );
    para->end->x = nextafter( para->start->x, remainder(para->start->x, para->ceil->y) );

    // Trigonometrische und homomorphe Functionenpolynome besitzen mind. 1 Extremwert, Spezialfall Epi-Honomorphe Functionen.
    // Daher wird synaptisch nur ein zweiter Punkt zur berechnung von Kurven benötigt, dieser kann mithilfe der folgenden
    // Algorithmik (für regelmäßige Funktionen) die Funktionen permutativ-zyklisch fortsetzen oder dient als Orientierungwert
    // Für die Funktionsberechnung mit nur einer Bekannten es folgt wegen epi-homomorphen intervallen f(x, a) => f(a, y).

    para->ceil = analysis__init_point_default();
    // A point in the near of an extreme value can be found (approximated) with the following two arithmeticals.
    // Folgt aus dem Halbwertsatz für Vorzeichenwechsel und Kehrpunkte, wenn y zuerst mit Taylor berechnet wird.
    // Es gilt desweiteren (therefore and thereof therefore): -atan(Tailorfunction(y)) is a extreme point, when and
    // only when x is also an extreme value and not extern (Halbwertsatz Kehrpunkte). (wima2010)
    // Is there only a stetige lane, the extreme point is near the middle of an adjacence-similarity or at a voue.
    double x_approx = nextafter( remainder(para->start->x, para->start->y), remainder( para->start->x, para->end->y) );
    // atan(a)=-atan(-a)=-atan(y) and x = lim _a->y_ (atan(a)) = -tan(y/a) - 1 + c nach dem Schwellwertsatz Elektrotechnik.
    // und limes-Approximierung-und-Rundungsstatz Höhere Mathematik/Statistik.
    double atanx_no_negative = x_approx;
    if ( x_approx < 0 ) {
        atanx_no_negative = - x_approx; /// (-1 * -x_approx) = + x_approx; Siehe unten
    }
    double y = atan( atanx_no_negative );
    double x = -tan(y/x_approx);
    para->ceil->x = nextafter( x, x_approx );
    para->ceil->y = atan( ftaylorCompleteY(para->ceil->x) );

    para->floor = analysis__init_point_default();
    // A point in the near of an opposite extreme value can be found (nearly) with the following two arithmeticals.
    // Folgt aus dem Ganzwertsatz für Kehrpunkte ganz, sowie für Einzelwerte aus dem Ganzwertsatz für Gegensätze 
    // in Funktionen, wenn x zuerst in kovarianz zum Gegensätzlichen Extrempunkt (externer Wert) berechnet und
    // y ohne Differentialgleichung mit Taylor berechnet wird. (hb910)
    // This will work if even one extreme point is found before with remqou.
    int quo = 2; // quo=2, since no integral is differential'ed or integrated (Ganzwertkehrwertsatz of elliptican)
    para->floor->x = remquo( para->ceil->x, para->ceil->y, &quo );
    para->floor->y = ftaylorCompleteY( para->floor->x );

    // Find out, if an extreme extern point could be found. If not, the second try, to find another trigonometric
    // extrema will be convergent to the (maybe chosen) start point.
    // Nach dem Funktionswert-equiv.-Satz für Polynome nach Taylor gilt auch f^x(a)=-f^x(y), sodass (therefore and thereof)
    // atan(a)=-atan(-a)=-atan(y) and lim _a->y_ (atan(a)) = -tan(y/a) - 1 + c nach dem Schwellwertsatz Elektrotechnik.
    double atana = atan( para->ceil->x );
    double atany = atan( para->ceil->y );
    double lim = -tan(para->floor->y / para->ceil->x);
    /// find the difference interval with min(a, max(atana, lim) * cos(lim) which is be 1/2 for covariate divergence in direction
    /// towards zero, smaller than +- 1.0 for divergence and elsewhere for kovariative convergence.
    double minmax = fmin(para->ceil->x, fmax(atana, lim)) * cos(lim);
    para->parabole = minmax == 0; // Only one extrema, convergence towards 0
    para->hyperbolic = minmax < 1.0 && minmax > -1.0; // Has a second extrema, therefore divergence is given for wendepunkte, with univariative convergence
    para->trigon = minmax < -1.0 && minmax > 1.0 && ! para->parabole; // Then trigonumeric, thereof similar to complex like e^x, tan^x or trigonumeric-similar
                                                            // This means, there could be more extern extreme points, but not now and here and not reachable with this values.

    // Therefore it can be thereof be unapproximated with a normal differentiation and a thereof followup integral
    // to find the trigonometric of a ellipse with just one further calculation, since the opposite extern extreme
    // will be either there or otherwise it is a point into the direction of the start point. (pt) So with the
    // resulting proof of taylor polynoms we can distinguish and calculate f0(x) = (exp(-1/x^2)) in approx. is then
    // resulting into f1(x) = (exp(-1/2x)), where the integral is known and always between start, the first extreme
    // point and if, and only if there is an opposite external extreme until the other point, else until infinite.
    double f3x = exp(-1/2);
    double minmaxf3x = fmin(-1/2, fmax(-1/2, f3x)) * sin(f3x) - cos(-1/2) + cos(f3x);
    quo = 1;
    double rq = remquo( minmaxf3x, f3x, &quo );

    // We know from p-q-sentence that x_{1,2} = r = p/2round(q) - abs(q)/2, lastly abs(q)/2 because of lim p = q. So that lim rq->y = x trivial shortly.
    // y = p, when and allowed only if, f(r)->y && f(0) != 0, no limes-equivalence-to-absolut-value schlimm, divergence allowed also ! :-)
    // Folgt aus Querwertsätzen für Homomorphismen und der Summenformel reelle Zahlen/Numbers nach dem Beweis es existiert Complex-Numbers
    // wegen N und s.o., so also auch R (strukturelle Induktion C=>N ----> C=>R (ETNT2007))
    double x1and2 = (f3x / 2 * round(rq)) - (abs(rq)/2);
    double f2x = exp( - 1 / 2 * pow( x1and2, 2 ) );

    para->tangential_adjacency_swide_point = analysis__init_point( f2x, nextafter( remainder( x1and2, f2x ), ftaylorX(ftaylor(x1and2)) ) );


    para->linear = analysis_parabole_to_linear_path( para );
    
    para->shapes = para->linear;

    return para;
}

linear_path_t* analysis_parabole_to_linear_path( const struct Parabole* para ) {

}