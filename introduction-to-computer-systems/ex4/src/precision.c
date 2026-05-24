#include <stdio.h>
#include <stdint.h>

typedef union { unsigned u; float f; } U;

int main(void)
{
    float F_data1 = 61.419997f;
    float F_data2 = 61.419998f;
    float F_data3 = 61.419999f;
    float F_data4 = 61.42f;
    float F_data5 = 61.420001f;

    printf("The number is:%.6f\n", F_data1);
    printf("The number is:%.6f\n", F_data2);
    printf("The number is:%.6f\n", F_data3);
    printf("The number is:%.6f\n", F_data4);
    printf("The number is:%.6f\n", F_data5);

            U u1 = { .f = F_data1 };
            U u2 = { .f = F_data2 };
            U u3 = { .f = F_data3 };
            U u4 = { .f = F_data4 };
            U u5 = { .f = F_data5 };

            printf("bits: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
                u1.u, u2.u, u3.u, u4.u, u5.u);

    return 0;
}
