#include <stdio.h>

int main()
{
    int value = 0x12345678;
    char *ptr = (char *)&value;
    
    printf("Address: %p\n", (void *)&value);
    printf("Byte 0: 0x%02X\n", ptr[0]);
    printf("Byte 1: 0x%02X\n", ptr[1]);
    printf("Byte 2: 0x%02X\n", ptr[2]);
    printf("Byte 3: 0x%02X\n", ptr[3]);
    
    return 0;
}
