import java.util.InputMismatchException;
import java.util.Scanner;

public class DivisionCalculator {
    public static void main(String[] args) {
        System.out.printf("=== 测试1：合法除法 (10/2) ===%n");
        testDivision("10", "2");

        System.out.printf("%n=== 测试2：非整数输入 (abc/2) ===%n");
        testDivision("abc", "2");

        System.out.printf("%n=== 测试3：除数为零 (10/0) ===%n");
        testDivision("10", "0");
    }

    public static void testDivision(String aStr, String bStr) {
        try {
            Scanner scanner = new Scanner(aStr);
            int a = scanner.nextInt();
            scanner.close();

            scanner = new Scanner(bStr);
            int b = scanner.nextInt();
            scanner.close();

            int result = a / b;
            System.out.printf("%d / %d = %d%n", a, b, result);
        } catch (InputMismatchException e) {
            System.out.printf("输入错误：请输入整数！%n");
        } catch (ArithmeticException e) {
            System.out.printf("数学错误：除数不能为零！%n");
        } finally {
            System.out.printf("程序结束%n");
        }
    }
}
