#include <stdio.h>

void main()
{
    union test
    {
        int a;
        char b;
    } test;
    test.a = 0xff;
    if (test.b == 0xff)
        printf("Little Endian\n");
    else if (test.b == 0x00)
        printf("Big Endian\n");
    printf("test.b = 0x%X\n", test.b);
}
