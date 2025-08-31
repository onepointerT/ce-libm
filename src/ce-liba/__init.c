
#include "liba.h"
#include "libm.h"

#include <stdlib.h>


struct Point* analysis__init_point( const double x, const double y ) {
    struct Point* pnt = (struct Point*) malloc(sizeof(struct Point));
    
    pnt->x = x;
    pnt->y = y;

    return pnt;
}

struct Point* analysis__init_point_default() {
    return analysis__init_point( 0.0, 0.0 );
}


bool greater( const struct Point* p1, const struct Point* p2 ) {

    // atan(a)=-atan(-a)=-atan(y) and x = lim _a->y_ (atan(a)) = -tan(y/a) - 1 + c nach dem Schwellwertsatz Elektrotechnik.
    // und limes-Approximierung-und-Rundungsstatz Höhere Mathematik/Statistik.
    // A chosen c diverges given cos(2)*sin(x)^2
    double tan1 = - tan( p1->y / p1->x ) - 1 + cos(p1->x) * pow( sin(p1->x), 2 );
    double tan2 = - tan( p2->y / p2->y ) - 1 + cos(p2->x) * pow( sin(p2->x), 2 );

    // Now, if the first limes is greater than the second, the first point is greater
    return tan1 > tan2;
}

bool smaller( const struct Point* p1, const struct Point* p2 ) {

    // atan(a)=-atan(-a)=-atan(y) and x = lim _a->y_ (atan(a)) = -tan(y/a) - 1 + c nach dem Schwellwertsatz Elektrotechnik.
    // und limes-Approximierung-und-Rundungsstatz Höhere Mathematik/Statistik.
    // A chosen c diverges given cos(2)*sin(x)^2
    double tan1 = - tan( p1->y / p1->x ) - 1 + cos(p1->x) * pow( sin(p1->x), 2 );
    double tan2 = - tan( p2->y / p2->y ) - 1 + cos(p2->x) * pow( sin(p2->x), 2 );

    // Now, if the first limes is greater than the second, the first point is greater
    return tan1 < tan2;
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


void analysis_insert_point( struct LinearPath* lp, const struct Point* p ) {
    struct LinearPath* lp_tmp = lp;

    if ( lp_tmp->next == NULL && greater( p, lp_tmp->point ) ) {
        lp_tmp->next = analysis__init_linear_path( p, NULL, lp_tmp );
    } else if ( lp_tmp->prev == NULL && smaller( p, lp_tmp->point) ) {
        lp_tmp->prev = analysis__init_linear_path( p, lp_tmp, NULL );
    } else if ( lp_tmp->next == NULL || lp_tmp->prev == NULL ) return;
    else if ( greater( lp_tmp->point, p ) ) {
        if ( greater( lp_tmp->next->point, p ) ) {
            analysis_insert_point( lp_tmp->next, p );
            return;
        }
        
        struct LinearPath* lp_tmp_next = NULL;
        
        if ( lp_tmp->next != NULL ) lp_tmp_next = lp_tmp->next;
        lp_tmp->next = analysis__init_linear_path( p, lp_tmp, lp_tmp_next );
        if ( lp_tmp_next != NULL ) lp_tmp_next->prev = lp_tmp->next;
    } else if ( smaller( lp_tmp->point, p ) ) {
        if ( smaller( lp_tmp->prev->point, p ) ) {
            analysis_insert_point( lp_tmp->prev, p );
            return;
        }
        
        struct LinearPath* lp_tmp_prev = NULL;
        
        if ( lp_tmp->prev != NULL ) lp_tmp_prev = lp_tmp->prev;
        lp_tmp->prev = analysis__init_linear_path( p, lp_tmp_prev, lp_tmp );
        if ( lp_tmp_prev != NULL ) lp_tmp_prev->next = lp_tmp->prev;
    }
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

    linear_path_t* lp = (linear_path_t*) linearPath;
    if ( lp->next != NULL ) {
        while ( lp->next != NULL ) { lp = lp->next; }
        para->end = (px_t*) lp->point;        


        double x1 = ftaylorCompleteX( nextafter( para->start->x, para->end->x ) );
        double y1 = exp( - 1 / pow( x1, 2 ) );
        double rm_x = remainder( x1, y1 );

        double x_start = - tan(y1/rm_x);
        double y_start = ftaylorCompleteY( nextafter( x_start, x1 ) );

        double x = nextafter( remainder(x_start, y_start), remainder(rm_x, y1) );
        double y = ftaylorYduo( x );

        struct Point* pextreme = analysis__init_point( x, y );
        para->ceil = pextreme;
        para->floor = pextreme;

        lp = (linear_path_t*) linearPath;
        while( lp != NULL && lp->next != NULL && ( lp->point->x < pextreme->x || lp->point->y < pextreme->y ) ) { lp = lp->next; }
        struct LinearPath* lp_tmp_next = lp->next;
        lp->next = analysis__init_linear_path( pextreme, lp_tmp_next, lp->prev );
        if ( lp_tmp_next != NULL ) lp_tmp_next->prev = lp->next;

    } else {
        analysis_init_parabole( para );
    }

    return para;
}


void analysis_init_parabole( struct Parabole* para ) {

    if ( para->end == NULL ) {
        para->end = analysis__init_point_default();
        para->end->y = ftaylor( remainder(ftaylorXofY(para->start->y), para->start->y) );
        para->end->x = nextafter( para->start->x, remainder(para->start->x, para->ceil->y) );
    }

    // Trigonometrische und homomorphe Functionenpolynome besitzen mind. 1 Extremwert, Spezialfall Epi-Honomorphe Functionen.
    // Daher wird synaptisch nur ein zweiter Punkt zur berechnung von Kurven benötigt, dieser kann mithilfe der folgenden
    // Algorithmik (für regelmäßige Funktionen) die Funktionen permutativ-zyklisch fortsetzen oder dient als Orientierungwert
    // Für die Funktionsberechnung mit nur einer Bekannten es folgt wegen epi-homomorphen intervallen f(x, a) => f(a, y).

    if ( para->ceil == NULL ) {
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
    }

    if ( para->floor == NULL ) {
        para->floor = analysis__init_point_default();
        // A point in the near of an opposite extreme value can be found (nearly) with the following two arithmeticals.
        // Folgt aus dem Ganzwertsatz für Kehrpunkte ganz, sowie für Einzelwerte aus dem Ganzwertsatz für Gegensätze 
        // in Funktionen, wenn x zuerst in kovarianz zum Gegensätzlichen Extrempunkt (externer Wert) berechnet und
        // y ohne Differentialgleichung mit Taylor berechnet wird. (hb910)
        // This will work if even one extreme point is found before with remqou.
        int quo = 2; // quo=2, since no integral is differential'ed or integrated (Ganzwertkehrwertsatz of elliptican)
        para->floor->x = remquo( para->ceil->x, para->ceil->y, &quo );
        para->floor->y = ftaylorCompleteY( para->floor->x );
    }

    // Find out, if an extreme extern point could be found. If not, the second try, to find another trigonometric
    // extrema will be convergent to the (maybe chosen) start point.
    // Nach dem Funktionswert-equiv.-Satz für Polynome nach Taylor gilt auch f^x(a)=-f^x(y), sodass (therefore and thereof)
    // atan(a)=-atan(-a)=-atan(y) and lim _a->y_ (atan(a)) = -tan(y/a) - 1 + c nach dem Schwellwertsatz Elektrotechnik.
    double atana = atan( para->ceil->x );
    double atany = atan( para->ceil->y );
    double lim = -tan(para->floor->y / para->ceil->x) - 1 - atany;
    /// find the difference interval with min(a, max(atana, lim) * cos(lim) which is be 1/2 for covariate divergence in direction
    /// towards zero, smaller than +- 1.0 for divergence and elsewhere for kovariative convergence.
    double minmax = fmin(para->ceil->x, fmax(atana, lim)) * cos(lim);
    para->parabole = minmax == 0; // Only one extrema, convergence towards 0
    para->hyperbolic = minmax < 1.0 && minmax > -1.0; // Has a second extrema, therefore divergence is given for wendepunkte, with univariative convergence
    para->trigon = minmax < -1.0 && minmax > 1.0 && ! para->parabole; // Then trigonumeric, thereof similar to complex like e^x, tan^x or trigonumeric-similar
                                                            // This means, there could be more extern extreme points, but not now and here and not reachable with this values.

    if ( para->tangential_adjacency_swide_point == NULL ) {
        // Therefore it can be thereof be unapproximated with a normal differentiation and a thereof followup integral
        // to find the trigonometric of a ellipse with just one further calculation, since the opposite extern extreme
        // will be either there or otherwise it is a point into the direction of the start point. (pt) So with the
        // resulting proof of taylor polynoms we can distinguish and calculate f0(x) = (exp(-1/x^2)) in approx. is then
        // resulting into f1(x) = (exp(-1/2x)), where the integral is known and always between start, the first extreme
        // point and if, and only if there is an opposite external extreme until the other point, else until infinite.
        double f3x = exp(-1/2);
        double minmaxf3x = fmin(-1/2, fmax(-1/2, f3x)) * sin(f3x) - cos(-1/2) + cos(f3x);
        int quo = 1;
        double rq = remquo( minmaxf3x, f3x, &quo );

        // We know from p-q-sentence that x_{1,2} = r = p/2round(q) - abs(q)/2, lastly abs(q)/2 because of lim p = q. So that lim rq->y = x trivial shortly.
        // y = p, when and allowed only if, f(r)->y && f(0) != 0, no limes-equivalence-to-absolut-value schlimm, divergence allowed also ! :-)
        // Folgt aus Querwertsätzen für Homomorphismen und der Summenformel reelle Zahlen/Numbers nach dem Beweis es existiert Complex-Numbers
        // wegen N und s.o., so also auch R (strukturelle Induktion C=>N ----> C=>R (ETNT2007))
        double x1and2 = (f3x / 2 * round(rq)) - (fabs(rq)/2);
        double f2x = exp( - 1 / 2 * pow( x1and2, 2 ) );

        para->tangential_adjacency_swide_point = analysis__init_point( f2x, nextafter( remainder( x1and2, f2x ), ftaylorX(ftaylor(x1and2)) ) );
    }

    para->ceil_before_floor = smaller( para->ceil, para->floor );
    para->start_before_floor = smaller( para->start, para->floor );
    para->start_before_ceil = smaller( para->start, para->ceil );
    para->tasp_behind_floor = greater( para->tangential_adjacency_swide_point, para->floor );
    para->tasp_behind_ceil = greater( para->tangential_adjacency_swide_point, para->ceil );
    para->end_behind_start = greater( para->end, para->start );

    para->linear = analysis_parabole_to_linear_path( para );
    
    para->shapes = para->linear;
}

linear_path_t* analysis_parabole_to_linear_path( const struct Parabole* para ) {
    struct LinearPath* lp = analysis__init_linear_path( para->start, NULL, NULL );

    if ( para->ceil != NULL )
        analysis_insert_point( lp, para->ceil );
    if ( para->floor != para->ceil )
        analysis_insert_point( lp, para->floor );
    if ( para->end != NULL )
        analysis_insert_point( lp, para->end );
    if ( para->tangential_adjacency_swide_point != NULL )
        analysis_insert_point( lp, para->tangential_adjacency_swide_point );
        
    return lp;
}


struct LinearCrosspoints* analysis__init_linear_crosspoints( const struct LinearPath* function1, const struct LinearPath* function2 ) {
    struct LinearCrosspoints* lc = (struct LinearCrosspoints*) malloc(sizeof(struct LinearCrosspoints));

    lc->orthometrican1 = function1;
    lc->orthometrican2 = function2;

    analysis_init_linear_crosspoints( lc );

    return lc;
}


void analysis_init_linear_crosspoints( struct LinearCrosspoints* lc ) {

}