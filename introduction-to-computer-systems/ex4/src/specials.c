#include <stdio.h>
#include <stdint.h>

typedef union { unsigned u; float f; } U;

int main(void)
{
    float finf1 = 4e38f, finf2 = 5e38f, finf3 = 6e38f;
    float fninf1 = -4e38f, fninf2 = -5e38f, fninf3 = -6e38f;
    float fzero = 0.0f, fnzero = -fzero;
    float fnormal1 = 5.0f, fnormal2 = 0.1f, fnnormal1 = -5.0f, fnnormal2 = -0.1f;
    float ffrac = 1e-40f, fnfrac = -1e-40f;
    float fnan1 = finf1 + fninf1, fnan2 = -fnan1;
    float finf4 = fnormal1 / fzero;

    printf("%f %f %f\n", finf1, finf2, finf3);
    printf("%f %f %f\n", fninf1, fninf2, fninf3);
    printf("%f %.20f \n%f %.20f\n", fnormal1, fnormal2, fnnormal1, fnnormal2);
    printf("%.50f\n%.50f\n", ffrac, fnfrac);
    printf("%f %f\n", fzero, fnzero);
    printf("%f %f\n", fnan1, fnan2);
    printf("%f \n", finf4);

    U t;
    t.f = finf1; printf("finf1 0x%08x\n", t.u);
    t.f = finf2; printf("finf2 0x%08x\n", t.u);
    t.f = finf3; printf("finf3 0x%08x\n", t.u);
    t.f = fninf1; printf("fninf1 0x%08x\n", t.u);
    t.f = fninf2; printf("fninf2 0x%08x\n", t.u);
    t.f = fninf3; printf("fninf3 0x%08x\n", t.u);
    t.f = fzero; printf("fzero 0x%08x\n", t.u);
    t.f = fnzero; printf("fnzero 0x%08x\n", t.u);
    t.f = fnormal1; printf("fnormal1 0x%08x\n", t.u);
    t.f = fnormal2; printf("fnormal2 0x%08x\n", t.u);
    t.f = fnnormal1; printf("fnnormal1 0x%08x\n", t.u);
    t.f = fnnormal2; printf("fnnormal2 0x%08x\n", t.u);
    t.f = ffrac; printf("ffrac 0x%08x\n", t.u);
    t.f = fnfrac; printf("fnfrac 0x%08x\n", t.u);
    t.f = fnan1; printf("fnan1 0x%08x\n", t.u);
    t.f = fnan2; printf("fnan2 0x%08x\n", t.u);
    t.f = finf4; printf("finf4 0x%08x\n", t.u);

    return 0;
}
