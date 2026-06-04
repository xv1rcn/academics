import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input n: ");
        int n = scanner.nextInt();
        scanner.close();

        int counter = 0;
        int increment = 1;
        while (counter + increment < n) {
            counter = counter + increment;
            ++increment;
        }
        int position = n - counter;

        int numerator, denominator;
        if (increment % 2 == 1) {
            numerator = increment - position + 1;
            denominator = position;
        } else {
            numerator = position;
            denominator = increment - position + 1;
        }
        System.out.printf("%d/%d", numerator, denominator);
    }
}
