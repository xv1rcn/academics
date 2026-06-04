import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input i: ");
        int i = scanner.nextInt();
        System.out.print("Please input j: ");
        int j = scanner.nextInt();
        scanner.close();

        int answer = 0;
        for (int number = i; number <= j; ++number) {
            answer += number;
            if (number != i) {
                System.out.print("+");
            }
            System.out.printf("%d", number);
        }
        System.out.printf("=%d", answer);
    }
}
