import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Please input n: ");
        int n = scanner.nextInt();
        scanner.close();

        long factorial = 1;
        long answer = 0;
        for (int number = 1; number <= n; ++number) {
            factorial = (factorial * number) % 1_000_000;
            answer = (answer + factorial) % 1_000_000;
        }
        System.out.print(answer);
    }
}
