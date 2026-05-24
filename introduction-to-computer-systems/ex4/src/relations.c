#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    int i1 = (1<<24);
    int i2 = (1<<24) + 1;
    printf("i=%d, (int)(float)i=%d -> %s\n", i1, (int)((float)i1), (i1 == (int)(float)i1) ? "equal" : "diff");
    printf("i=%d, (int)(float)i=%d -> %s\n", i2, (int)((float)i2), (i2 == (int)(float)i2) ? "equal" : "diff");

    float f1 = 3.5f;
    printf("f=%f, (float)(int)f=%f -> %s\n", f1, (float)((int)f1), (f1 == (float)(int)f1) ? "true" : "false");

    int ia = INT_MAX/2;
    printf("i=%d, (int)(double)i=%d -> %s\n", ia, (int)((double)ia), (ia == (int)(double)ia) ? "true" : "false");

    float fb = 1.234567f;
    printf("f=%f, (float)(double)f=%f -> %s\n", fb, (float)((double)fb), (fb == (float)(double)fb) ? "true" : "false");

    double d1 = 1.23456789012345;
    printf("d=%0.15g, (float)d=%0.9g -> %s\n", d1, (float)d1, (d1 == (float)d1) ? "true" : "false");

    float f2 = -0.0f;
    printf("f=%f, -(-f)=%f -> %s\n", f2, -(-f2), (f2 == -(-f2)) ? "true" : "false");

    double large = 1e308;
    printf("d=%g, d+d-d = %g -> %s\n", large, large+large-large, (large+large-large == large) ? "true" : "false");

    return 0;
}
