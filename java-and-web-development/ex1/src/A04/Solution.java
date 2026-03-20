import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input n: ");
        int n = scanner.nextInt();
        scanner.close();

        int answer = 0;
        for (int number = 1; number <= n; number += 2) {
            answer += number;
            if (number != 1) {
                System.out.print("+");
            }
            System.out.printf("%d", number);
        }
        System.out.printf("=%d", answer);
    }
}
