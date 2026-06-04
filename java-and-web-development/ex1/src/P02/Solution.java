import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input n: ");
        int n = scanner.nextInt();
        System.out.print("Please input t: ");
        int t = scanner.nextInt();
        System.out.print("Please input ch: ");
        char ch = scanner.next().charAt(0);
        scanner.close();

        for (int row = 1; row <= n; ++row) {
            for (int column = 0; column < n - row; ++column) {
                System.out.print(" ");
            }
            for (int column = 0; column < 2 * row - 1; ++column) {
                System.out.print(ch);
            }
            System.out.println();
        }
        for (int row = 0; row < t; ++row) {
            for (int column = 0; column < n - 1; ++column) {
                System.out.print(" ");
            }
            System.out.printf("%c\n", ch);
        }
    }
}
