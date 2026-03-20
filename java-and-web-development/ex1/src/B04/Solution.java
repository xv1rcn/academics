import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input a: ");
        int a = scanner.nextInt();
        System.out.print("Please input b: ");
        int b = scanner.nextInt();
        System.out.print("Please input c: ");
        int c = scanner.nextInt();
        scanner.close();

        for (int number = 10; number < 100; ++number) {
            if (number % 3 == a && number % 5 == b && number % 7 == c) {
                System.out.printf("Possible answer: %d", number);
                return;
            }
        }
        System.out.println("Possible answer not found...");
    }
}
