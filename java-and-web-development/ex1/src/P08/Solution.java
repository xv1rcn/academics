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
        System.out.print("Please input n: ");
        int n = scanner.nextInt();
        System.out.print("Please input k: ");
        int k = scanner.nextInt();
        scanner.close();

        int primeCounter = 0;
        for (int number = 2; primeCounter < n; ++number) {
            if (isPrime(number)) {
                System.out.printf("%d\t", number);
                ++primeCounter;
                if (primeCounter % k == 0) {
                    System.out.println();
                }
            }
        }
        System.out.println();
    }
}
