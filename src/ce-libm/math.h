#pragma once

#include <float.h>

#undef _CMATH_
#undef _INC_LIMITS

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NAN
#define NAN (0.0 / 0.10)
#endif

#ifndef INFINITY
#define INFINITY (1.0 / 0.10)
#endif

int abs(int x);
long labs(long j);
long long llabs(long long x);

double fabs(double x);
float fabsf(float x);
long double fabsl(long double x);

#define M_PI (3.14159265358979323846264338327f)
#define M_PI_2 (M_PI / 2.0)
#define M_PI_4 (M_PI / 4.0)

#define M_1_PI 0.31830988618379067154 /* 1/pi */
#define M_2_PI (M_PI * 2.0)
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */

#if defined(_XHJ_OPEN_SOURCE) || defined(_GNU_SOURCE) ||                       \
    defined(_BSD_SOURCE) || defined(_SKIFT_SOURCE)
#define M_E 2.7182818284590452354       /* e */
#define M_LOG2E 1.4426950408889634074   /* log_2 e */
#define M_LOG10E 0.43429448190325182765 /* log_10 e */
#define M_LN2 0.69314718055994530942    /* log_e 2 */
#define M_LN10 2.30258509299404568402   /* log_e 10 */

#define M_SQRT2 1.41421356237309504880   /* sqrt(2) */
#define M_SQRT1_2 0.70710678118654752440 /* 1/sqrt(2) */

extern int signgam;

double j0(double);
double j1(double);
double jn(int, double);

double y0(double);
double y1(double);
double yn(int, double);

#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) || defined(_SKIFT_SOURCE)
#define HUGE 3.40282346638528859812e+38F

double drem(double, double);
float dremf(float, float);

int finite(double);
int finitef(float);

double scalb(double, double);
float scalbf(float, float);

double significand(double);
float significandf(float);

double lgamma_r(double, int *);
float lgammaf_r(float, int *);

float j0f(float);
float j1f(float);
float jnf(int, float);

float y0f(float);
float y1f(float);
float ynf(int, float);

#endif

typedef double double_t;
typedef float float_t;

#define HUGE_VAL (__builtin_huge_val())
#define FP_ILOGBNAN (-1 - (int)(((unsigned)-1) >> 1))
#define FP_ILOGB0 FP_ILOGBNAN

#define signbit(x) (__builtin_signbit(x))

// [C11/7.12.3 Classification macros]

// NOTE: fpclassify always returns exactly one of those constants
// However making them bitwise disjoint simplifies isfinite() etc.
#define FP_INFINITE 1
#define FP_NAN 2
#define FP_NORMAL 4
#define FP_SUBNORMAL 8
#define FP_ZERO 16

int __fpclassify(double x);
int __fpclassifyf(float x);
int __fpclassifyl(long double x);

#define fpclassify(x)                                                          \
  (sizeof(x) == sizeof(double)                                                 \
       ? __fpclassify(x)                                                       \
       : (sizeof(x) == sizeof(float)                                           \
              ? __fpclassifyf(x)                                               \
              : (sizeof(x) == sizeof(long double) ? __fpclassifyl(x) : 0)))

#define isfinite(x) (fpclassify(x) & (FP_NORMAL | FP_SUBNORMAL | FP_ZERO))
#define isnan(x) (fpclassify(x) == FP_NAN)
#define isinf(x) (fpclassify(x) == FP_INFINITE)
#define isnormal(x) (fpclassify(x) == FP_NORMAL)

// [ GNUC extension]

void sincos(double, double *, double *);
void sincosf(float, float *, float *);
void sincosl(long double, long double *, long double *);

double exp10(double);
float exp10f(float);
long double exp10l(long double);

double pow10(double);
float pow10f(float);
long double pow10l(long double);

// [C11/7.12.4 Trigonometric functions]

double acos(double x);
float acosf(float x);
long double acosl(long double x);
double asin(double x);
float asinf(float x);
long double asinl(long double x);
double atan(double x);
float atanf(float x);
long double atanl(long double x);
double atan2(double x, double y);
float atan2f(float x, float y);
long double atan2l(long double x, long double y);
double cos(double x);
float cosf(float x);
long double cosl(long double x);
double sin(double x);
float sinf(float x);
long double sinl(long double x);
double tan(double x);
float tanf(float x);
long double tanl(long double x);

// [C11/7.12.5 Hyperbolic functions]

double acosh(double x);
float acoshf(float x);
long double acoshl(long double x);

double asinh(double x);
float asinhf(float x);
long double asinhl(long double x);

double atanh(double x);
float atanhf(float x);
long double atanhl(long double x);

double cosh(double x);
float coshf(float x);
long double coshl(long double x);

double sinh(double x);
float sinhf(float x);
long double sinhl(long double x);

double tanh(double x);
float tanhf(float x);
long double tanhl(long double x);

// [C11/7.12.6 Exponential and logarithmic functions]

double exp(double x);
float expf(float x);
long double expl(long double x);

double exp2(double x);
float exp2f(float x);
long double exp2l(long double x);

double expm1(double x);
float expm1f(float x);
long double expm1l(long double x);

double frexp(double x, int *power);
float frexpf(float x, int *power);
long double frexpl(long double x, int *power);

int ilogb(double x);
int ilogbf(float x);
int ilogbl(long double x);

double ldexp(double x, int power);
float ldexpf(float x, int power);
long double ldexpl(long double x, int power);

double log(double x);
float logf(float x);
long double logl(long double x);

double log10(double x);
float log10f(float x);
long double log10l(long double x);

double log1p(double x);
float log1pf(float x);
long double log1pl(long double x);

double log2(double x);
float log2f(float x);
long double log2l(long double x);

double logb(double x);
float logbf(float x);
long double logbl(long double x);

double modf(double x, double *integral);
float modff(float x, float *integral);
long double modfl(long double x, long double *integral);

double scalbn(double x, int power);
float scalbnf(float x, int power);
long double scalbnl(long double x, int power);

double scalbln(double x, long power);
float scalblnf(float x, long power);
long double scalblnl(long double x, long power);

// [C11/7.12.7 Power and absolute-value functions]

double cbrt(double x);
float cbrtf(float x);
long double cbrtl(long double x);

double fabs(double x);
float fabsf(float x);
long double fabsl(long double x);

double hypot(double x, double y);
float hypotf(float x, float y);
long double hypotl(long double x, long double y);

double pow(double x, double y);
float powf(float x, float y);
long double powl(long double x, long double y);

double sqrt(double x);
float sqrtf(float x);
long double sqrtl(long double x);

// [C11/7.12.8 Error and gamma functions]

double erf(double x);
float erff(float x);
long double erfl(long double x);

double erfc(double x);
float erfcf(float x);
long double erfcl(long double x);

double lgamma(double x);
float lgammaf(float x);
long double lgammal(long double x);

double tgamma(double x);
float tgammaf(float x);
long double tgammal(long double x);

// [C11/7.12.9 Nearest integer functions]

double ceil(double x);
float ceilf(float x);
long double ceill(long double x);

double floor(double x);
float floorf(float x);
long double floorl(long double x);

double nearbyint(double x);
float nearbyintf(float x);
long double nearbyintl(long double x);

double rint(double x);
float rintf(float x);
long double rintl(long double x);

long lrint(double x);
long lrintf(float x);
long lrintl(long double x);

long long llrint(double x);
long long llrintf(float x);
long long llrintl(long double x);

double round(double x);
float roundf(float x);
long double roundl(long double x);

long lround(double x);
long lroundf(float x);
long lroundl(long double x);

long long llround(double x);
long long llroundf(float x);
long long llroundl(long double x);

double trunc(double x);
float truncf(float x);
long double truncl(long double x);

// [C11/7.12.10 Remainder functions]

double fmod(double x, double y);
float fmodf(float x, float y);
long double fmodl(long double x, long double y);

double remainder(double x, double y);
float remainderf(float x, float y);
long double remainderl(long double x, long double y);

double remquo(double x, double y, int *quotient);
float remquof(float x, float y, int *quotient);
long double remquol(long double x, long double y, int *quotient);

// [C11/7.12.11 Manipulation functions]

double copysign(double x, double sign);
float copysignf(float x, float sign);
long double copysignl(long double x, long double sign);

double nan(char const *tag);
float nanf(char const *tag);
long double nanl(char const *tag);

double nextafter(double x, double dir);
float nextafterf(float x, float dir);
long double nextafterl(long double x, long double dir);

double nexttoward(double x, long double dir);
float nexttowardf(float x, long double dir);
long double nexttowardl(long double x, long double dir);

// [C11/7.12.12 Maximum, minimum and positive difference functions]

double fdim(double x, double y);
float fdimf(float x, float y);
long double fdiml(long double x, long double y);

double fmax(double x, double y);
float fmaxf(float x, float y);
long double fmaxl(long double x, long double y);

double fmin(double x, double y);
float fminf(float x, float y);
long double fminl(long double x, long double y);

double clamp(double x, double minimum, double maximum);
float clampf(float x, float minimum, float maximum);
long double clampl(long double x, long double minimum, long double maximum);

// [C / Taylor polynoms]

double ftaylor( double x );
double ftaylorX( double ty );
double ftaylorY( double tx );
double ftaylorPolyX( double tx );
double ftaylorPolyY( double tx );
double ftaylorCompleteX( double x );
double ftaylorCompleteY( double x );
double ftaylorXofY( double y );
double ftaylorduo( double T1, double T2 );
double ftaylorXduo( double x );
double ftaylorYduo( double x );

// [C11/7.12.13 Floating multiply-add]

double fma(double, double, double);
float fmaf(float, float, float);
long double fmal(long double, long double, long double);


#pragma intrinsic(acos)
#pragma intrinsic(acosf)
#pragma intrinsic(asin)
#pragma intrinsic(asinf)
#pragma intrinsic(atan)
#pragma intrinsic(atan2)
#pragma intrinsic(atan2f)
#pragma intrinsic(atanf)
#pragma intrinsic(ceil)
#pragma intrinsic(ceilf)
#pragma intrinsic(cos)
#pragma intrinsic(cosf)
#pragma intrinsic(cosh)
#pragma intrinsic(coshf)
#pragma intrinsic(exp)
#pragma intrinsic(expf)
#pragma intrinsic(fabs)
#pragma intrinsic(floor)
#pragma intrinsic(floorf)
#pragma intrinsic(fma)
#pragma intrinsic(fmaf)
#pragma intrinsic(fmal)
#pragma intrinsic(fmod)
#pragma intrinsic(fmodf)
#pragma intrinsic(log)
#pragma intrinsic(logf)
#pragma intrinsic(lrint)
#pragma intrinsic(lrintf)
#pragma intrinsic(lrintl)
#pragma intrinsic(pow)
#pragma intrinsic(powf)
#pragma intrinsic(sin)
#pragma intrinsic(sinf)
#pragma intrinsic(sinh)
#pragma intrinsic(sinhf)
#pragma intrinsic(sqrt)
#pragma intrinsic(sqrtf)
#pragma intrinsic(tan)
#pragma intrinsic(tanh)
#pragma intrinsic(tanhf)

#ifdef __cplusplus
}
#endif
