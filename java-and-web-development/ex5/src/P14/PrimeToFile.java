import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class PrimeToFile {
    public static void main(String[] args) {
        int p = 2;
        int s = 20;
        int t = 3;

        List<Integer> primes = new ArrayList<>();
        for (int n = p; n <= s; n++) {
            if (isPrime(n)) {
                primes.add(n);
            }
        }

        new File("build").mkdirs();

        String filePath = "build/primes.txt";
        try (PrintWriter pw = new PrintWriter(new FileWriter(filePath))) {
            int count = 0;
            int lineSum = 0;
            StringBuilder lineBuilder = new StringBuilder();

            for (int prime : primes) {
                lineBuilder.append(prime).append(" ");
                lineSum += prime;
                count++;

                if (count == t) {
                    lineBuilder.append(" ").append(lineSum);
                    pw.println(lineBuilder.toString());
                    lineBuilder.setLength(0);
                    lineSum = 0;
                    count = 0;
                }
            }

            if (count > 0) {
                lineBuilder.append(" ").append(lineSum);
                pw.println(lineBuilder.toString());
            }

            System.out.printf("质数已写入 %s%n", filePath);
        } catch (IOException e) {
            System.out.printf("写入文件时出错: %s%n", e.getMessage());
        }
    }

    private static boolean isPrime(int n) {
        if (n < 2) {
            return false;
        }
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }
}
