import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input n: ");
        int n = scanner.nextInt();
        scanner.close();

        double answer = 0;
        boolean sign = true;
        for (int number = 1; number <= n; ++number) {
            if (sign) {
                answer += 1. / number;
            } else {
                answer -= 1. / number;
            }
            sign = !sign;
        }
        System.out.printf("Sum = %.6f", answer);
    }
}
