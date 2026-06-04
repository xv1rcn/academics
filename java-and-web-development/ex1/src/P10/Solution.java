import java.util.Scanner;

public class Solution {
    private static int gcd(int a, int b) {
        // return a == b ? a : a > b ? gcd(a - b, b) : gcd(a, b - a);
        return b == 0 ? a : gcd(b, a % b);
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input a: ");
        int a = scanner.nextInt();
        System.out.print("Please input b: ");
        int b = scanner.nextInt();
        scanner.close();

        System.out.print(gcd(a, b));
    }
}
