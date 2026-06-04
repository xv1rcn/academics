import java.util.Scanner;

public class Solution {
    private static boolean isPrime(int n) {
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input i: ");
        int i = scanner.nextInt();
        System.out.print("Please input j: ");
        int j = scanner.nextInt();
        System.out.print("Please input k: ");
        int k = scanner.nextInt();
        scanner.close();

        int newlineCounter = 0;
        for (int number = i; number <= j; ++number) {
            if (isPrime(number)) {
                System.out.printf("%d\t", number);
                ++newlineCounter;
                if (newlineCounter == k) {
                    System.out.println();
                    newlineCounter = 0;
                }
            }
        }
        System.out.println();
    }
}
