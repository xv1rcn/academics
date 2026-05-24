#include "add.h"

int add(int arr[], int n) {
    int sum = 0, i;
    for (i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}