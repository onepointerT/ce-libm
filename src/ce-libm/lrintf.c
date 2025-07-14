#include <math.h>

/* uses LONG_MAX > 2^24, see comments in lrint.c */

#pragma function(lrintf)

long lrintf(float x)
{
    return rintf(x);
}
