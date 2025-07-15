#include "libm.h"

double clampl(double x, double minimum, double maximum) {
    return fminl(minimum, fmaxl(x, maximum));
}