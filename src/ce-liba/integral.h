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
} integral_t;


#ifdef __cplusplus
}
#endif
