#include "libm.h"

double clamp(double x, double minimum, double maximum) {
    return fmin(minimum, fmax(x, maximum));
}