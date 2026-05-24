#include <stdio.h>

int main() {
    printf("0 == 0U is %s\n", (0 == 0U) ? "true" : "false");
    printf("-1 < 0 is %s\n", (-1 < 0) ? "true" : "false");
    printf("-1 < 0U is %s\n", (-1 < 0U) ? "true" : "false");

    printf("2147483647 > -2147483647 - 1 is %s\n",
           (2147483647 > -2147483647 - 1) ? "true" : "false");
    printf("2147483647U > -2147483647 - 1 is %s\n",
           (2147483647U > -2147483647 - 1) ? "true" : "false");

    printf("2147483647 > (int)2147483648U is %s\n",
           (2147483647 > (int)2147483648U) ? "true" : "false");

    printf("-1 > -2 is %s\n", (-1 > -2) ? "true" : "false");
    printf("(unsigned)-1 > -2 is %s\n",
           ((unsigned)-1 > -2) ? "true" : "false");

    return 0;
}