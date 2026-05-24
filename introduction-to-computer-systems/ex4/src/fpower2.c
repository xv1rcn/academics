#include <stdio.h>
#include <stdint.h>

float u2f(unsigned u)
{
    union { unsigned u; float f; } tmp;
    tmp.u = u;
    return tmp.f;
}

float fpower2(int x)
{
    unsigned exp, frac, u;
    if (x < -149) {
        exp = 0; frac = 0;
    } else if (x <= -127) {
        exp = 0; frac = 1u << (x + 149);
    } else if (x <= 127) {
        exp = (unsigned)(x + 127); frac = 0;
    } else {
        exp = 255; frac = 0;
    }
    u = (exp << 23) | frac;
    return u2f(u);
}

#ifdef TEST_FPOWER2
int main(void)
{
    for (int x = -155; x <= 130; x += 16) {
        float f = fpower2(x);
        union { unsigned u; float f; } t;
        t.f = f;
        printf("x=%4d -> f=%e (hex=0x%08x)\n", x, f, t.u);
    }
    return 0;
}
#endif
