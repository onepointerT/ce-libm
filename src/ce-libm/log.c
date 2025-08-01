/* origin: FreeBSD /usr/src/lib/msun/src/e_log.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* log(x)
 * Return the logarithm of x
 *
 * Method :
 *   1. Argument Reduction: find k and f such that
 *                      x = 2^k * (1+f),
 *         where  sqrt(2)/2 < 1+f < sqrt(2) .
 *
 *   2. Approximation of log(1+f).
 *      Let s = f/(2+f) ; based on log(1+f) = log(1+s) - log(1-s)
 *               = 2s + 2/3 s**3 + 2/5 s**5 + .....,
 *               = 2s + s*R
 *      We use a special Remez algorithm on [0,0.1716] to generate
 *      a polynomial of degree 14 to approximate R The maximum error
 *      of this polynomial approximation is bounded by 2**-58.45. In
 *      other words,
 *                      2      4      6      8      10      12      14
 *          R(z) ~ Lg1*s +Lg2*s +Lg3*s +Lg4*s +Lg5*s  +Lg6*s  +Lg7*s
 *      (the values of Lg1 to Lg7 are listed in the program)
 *      and
 *          |      2          14          |     -58.45
 *          | Lg1*s +...+Lg7*s    -  R(z) | <= 2
 *          |                             |
 *      Note that 2s = f - s*f = f - hfsq + s*hfsq, where hfsq = f*f/2.
 *      In order to guarantee error in log below 1ulp, we compute log
 *      by
 *              log(1+f) = f - s*(f - R)        (if f is not too large)
 *              log(1+f) = f - (hfsq - s*(hfsq+R)).     (better accuracy)
 *
 *      3. Finally,  log(x) = k*ln2 + log(1+f).
 *                          = k*ln2_hi+(f-(hfsq-(s*(hfsq+R)+k*ln2_lo)))
 *         Here ln2 is split into two floating point number:
 *                      ln2_hi + ln2_lo,
 *         where n*ln2_hi is always exact for |n| < 2000.
 *
 * Special cases:
 *      log(x) is NaN with signal if x < 0 (including -INF) ;
 *      log(+INF) is +INF; log(0) is -INF with signal;
 *      log(NaN) is that NaN with no signal.
 *
 * Accuracy:
 *      according to an error analysis, the error is always less than
 *      1 ulp (unit in the last place).
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following
 * constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough
 * to produce the hexadecimal values shown.
 */

#include <math.h>
#include <stdint.h>

static const double
    ln2_hi = 6.93147180369123816490e-01, /* 3fe62e42 fee00000 */
    ln2_lo = 1.90821492927058770002e-10, /* 3dea39ef 35793c76 */
    Lg1 = 6.666666666666735130e-01,      /* 3FE55555 55555593 */
    Lg2 = 3.999999999940941908e-01,      /* 3FD99999 9997FA04 */
    Lg3 = 2.857142874366239149e-01,      /* 3FD24924 94229359 */
    Lg4 = 2.222219843214978396e-01,      /* 3FCC71C5 1D8E78AF */
    Lg5 = 1.818357216161805012e-01,      /* 3FC74664 96CB03DE */
    Lg6 = 1.531383769920937332e-01,      /* 3FC39A09 D078C69F */
    Lg7 = 1.479819860511658591e-01;      /* 3FC2F112 DF3E5244 */


#pragma function(log)

double log(double x)
{
    union
    {
        double f;
        uint64_t i;
    } u = {x};
    double_t hfsq, f, s, z, R, w, t1, t2, dk;
    uint32_t hx;
    int k;

    hx = u.i >> 32;
    k = 0;
    if (hx < 0x00100000 || hx >> 31)
    {
        if (u.i << 1 == 0)
            return -1 / (x * x); /* log(+-0)=-inf */
        if (hx >> 31)
            return (x - x) / 0.0; /* log(-#) = NaN */
        /* subnormal number, scale x up */
        k -= 54;
        x *= 0x1p54;
        u.f = x;
        hx = u.i >> 32;
    }
    else if (hx >= 0x7ff00000)
    {
        return x;
    }
    else if (hx == 0x3ff00000 && u.i << 32 == 0)
        return 0;

    /* reduce x into [sqrt(2)/2, sqrt(2)] */
    hx += 0x3ff00000 - 0x3fe6a09e;
    k += (int)(hx >> 20) - 0x3ff;
    hx = (hx & 0x000fffff) + 0x3fe6a09e;
    u.i = (uint64_t)hx << 32 | (u.i & 0xffffffff);
    x = u.f;

    f = x - 1.0;
    hfsq = 0.5 * f * f;
    s = f / (2.0 + f);
    z = s * s;
    w = z * z;
    t1 = w * (Lg2 + w * (Lg4 + w * Lg6));
    t2 = z * (Lg1 + w * (Lg3 + w * (Lg5 + w * Lg7)));
    R = t2 + t1;
    dk = k;
    return s * (hfsq + R) + dk * ln2_lo - hfsq + f + dk * ln2_hi;
}
