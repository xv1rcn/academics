#include "printarray.h"

void printArray(int arr[], int n, char* str) {
    printf("%s", str);
    int i;
    for (i = 0; i < n; i++) {
        printf("%5d ", arr[i]);
    }
    printf("\n");
}