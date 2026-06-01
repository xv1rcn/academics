public class FinallyReturnDemo {
    public static int calculate() {
        try {
            return 1;
        } catch (Exception e) {
            return 2;
        } finally {
            System.out.printf("进入finally%n");
            /* return 3; */
        }
    }

    public static void main(String[] args) {
        int result = calculate();
        System.out.printf("返回值: %d%n", result);
    }
}
