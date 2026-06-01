import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class SquareContains {
    public static void main(String[] args) {
        int count = 0;
        String resPath = "build/result.txt";

        File buildDir = new File("build");
        if (!buildDir.exists()) {
            buildDir.mkdirs();
        }

        try (PrintWriter writer = new PrintWriter(new FileWriter(resPath))) {
            for (int n = 100; n <= 999; n++) {
                int square = n * n;
                String nStr = String.valueOf(n);
                String sqStr = String.valueOf(square);

                if (sqStr.contains(nStr)) {
                    writer.printf("%d²=%d 包含 %d%n", n, square, n);
                    count++;
                }
            }
        } catch (IOException e) {
            System.out.printf("文件写入错误: %s%n", e.getMessage());
            return;
        }

        System.out.printf("在100~999范围内，平方包含自身连续三位的数共有 %d 个%n", count);
    }
}
