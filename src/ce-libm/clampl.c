#include "libm.h"

long double clampl(long double x, long double minimum, long double maximum) {
    return fminl(minimum, fmaxl(x, maximum));
}