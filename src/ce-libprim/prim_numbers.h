#pragma once



#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>


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






#ifdef __cplusplus
}
#endif
