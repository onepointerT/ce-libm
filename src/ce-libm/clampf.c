#include "libm.h"

float clampf(float x, float minimum, float maximum) {
    return fminf(minimum, fmaxf(x, maximum));
}