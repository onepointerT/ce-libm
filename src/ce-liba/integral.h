#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>
#include <stdbool.h>

#include "functions.h"


typedef struct Integral {
    double limit_lower;
    double limit_upper;
    struct Function* function;
    struct Function* constant;
} integral_t;


struct Integral* analysis__init_integral( const double limit_low, const double limit_up, const char* fx, const char* cons );
double analysis_integral_calculate( const struct Integral* integral );


#ifdef __cplusplus
}
#endif
