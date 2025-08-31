#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>
#include <stdbool.h>



#ifndef NAN
#define NAN (0.0 / 0.10)
#endif

#ifndef INFINITY
#define INFINITY (1.0 / 0.10)
#endif


#define M_PI (3.14159265358979323846264338327f)
#define M_PI_2 (M_PI / 2.0)
#define M_PI_4 (M_PI / 4.0)

#define M_1_PI 0.31830988618379067154 /* 1/pi */
#define M_2_PI (M_PI * 2.0)
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */



typedef struct Point {
    double x;
    double y;
} px_t;


struct Point* analysis__init_point( const double x, const double y );
struct Point* analysis__init_point_default();
bool greater( const struct Point* p1, const struct Point* p2 );
bool smaller( const struct Point* p1, const struct Point* p2 );


typedef struct LinearPath {
    const struct Point* point;
    struct LinearPath* next;
    struct LinearPath* prev;
} linear_path_t;

struct LinearPath* analysis__init_linear_path( const struct Point* px, struct LinearPath* next, struct LinearPath* prev );
struct LinearPath* analysis__init_linear_path_default( const struct Point* px );
void analysis_insert_point( struct LinearPath* lp, const struct Point* p );

typedef struct Parabole {
    const struct Point* start;
    struct Point* end;
    struct LinearPath* linear;
    struct Point* ceil;
    struct Point* floor;
    struct LinearPath* shapes;

    bool hyperbolic;
    bool parabole;
    bool trigon;

    bool ceil_before_floor;
    bool start_before_floor;
    bool start_before_ceil;
    bool tasp_behind_floor;
    bool tasp_behind_ceil;
    bool end_behind_start;

    struct Point* tangential_adjacency_swide_point;
} parabole_t;

struct Parabole* analysis__init_parabole( const struct Point* px );
struct Parabole* analysis__init_parabole_from_linearPath( const linear_path_t* linearPath );
void analysis_init_parabole( struct Parabole* para );
linear_path_t* analysis_parabole_to_linear_path( const struct Parabole* para );


typedef double Angle;

typedef struct LinearCrosspoints {
    struct LinearPath* lp;
    struct Point* cp1;
    struct Point* cp2;
    const struct LinearPath* orthometrican1;
    const struct LinearPath* orthometrican2;
} linear_cp_t;

struct LinearCrosspoints* analysis__init_linear_crosspoints( const struct LinearPath* function1, const struct LinearPath* function2 );
void analysis_init_linear_crosspoints( struct LinearCrosspoints* lc );

typedef struct Linear3Orthometrican {
    struct LinearPath* left_cathesian;
    struct LinearPath* right_cathesian;

    const struct LinearPath* f;

    const Angle angle12; /// should be 90°
    Angle angle_left; /// ca. 45°
    Angle angle_right; /// ca. 45°

    const struct LinearPath* hyperbolic_linear_end;
} linear_3_ortho_t;

struct Linear3Orthometrican* analysis__init_linear_3_orthometrican( const struct LinearPath* function, const struct LinearPath* other_function );


typedef struct LinearConcatenation {
    const struct LinearPath* f1;
    struct Linear3Orthometrican* ortho1;

    const struct LinearPath* f2;
    struct Linear3Orthometrican* ortho2;

    struct LinearCrosspoints* linear_cp; // adjacent tangente with crosspoints

    struct Parabole* concat;
} linear_concat_t;

struct LinearConcatenation* analysis__init_linear_concatenation( const struct LinearPath* function, const struct LinearPath* other_function );


#ifdef __cplusplus
}
#endif
