public class ArrayConverter {
    private static final String[] arr = {"10", "20", "abc", "30"};

    public static void getIntegerFromArray(int index) {
        try {
            int result = Integer.parseInt(arr[index]);
            System.out.printf("索引%d的元素值为：%d%n", index, result);
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.printf("索引越界，有效范围为0~3%n");
        } catch (NumberFormatException e) {
            System.out.printf("数组元素不是有效整数%n");
        } finally {
            System.out.printf("方法执行完毕%n");
        }
    }

    public static void main(String[] args) {
        for (int i = 0; i <= 4; i++) {
            System.out.printf("=== 调用索引 %d ===%n", i);
            getIntegerFromArray(i);
            System.out.printf("%n");
        }
    }
}
