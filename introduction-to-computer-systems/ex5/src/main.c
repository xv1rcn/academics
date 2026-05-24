#include "add.h"
#include "bubblesort.h"
#include "printarray.h"

#define bool char
#define true 1
#define false 0

int main(void) {
    int a[10], b[10], i;
    printf("请输入 10 个整数：");
    for (i = 0; i < 10; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }

    bool flag = true;
    while (flag) {
        printf("1. 冒泡排序\n2. 求和\n3. 打印结果\n4. 退出\n");
        printf("请选择序号：");
        int choice;
        scanf("%d", &choice);

        int sum;
        switch (choice) {
            case 1:
                bubbleSort(a, 10);
                break;
            case 2:
                sum = add(a, 10);
                printf("数组求和结果为：%5d\n", sum);
                break;
            case 3:
                printArray(b, 10, "原始数组为：");
                printArray(a, 10, "排序后数组为：");
                break;
            case 4:
                flag = false;
                break;
            default:
                printf("请选择正确的序号！\n");
        }
    }

    return 0;
}