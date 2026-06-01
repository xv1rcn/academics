import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class ReadLineFile {
    public static void main(String[] args) {
        String filePath = "build/lines_test.txt";
        try {
            Path resDir = Paths.get("build");
            if (Files.notExists(resDir)) {
                Files.createDirectory(resDir);
            }
        } catch (IOException e) {
            System.out.printf("创建build目录失败: %s%n", e.getMessage());
            return;
        }
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filePath))) {
            writer.write("春眠不觉晓，处处闻啼鸟。");
            writer.newLine();
            writer.write("夜来风雨声，花落知多少。");
            writer.newLine();
            writer.write("Hello, World from Java!");
            writer.newLine();
            writer.write("BufferedReader readLine 逐行读取测试");
            writer.newLine();
            writer.write("The quick brown fox jumps over the lazy dog.");
            writer.newLine();
            System.out.printf("已创建测试文件: %s%n", filePath);
        } catch (IOException e) {
            System.out.printf("写入文件失败: %s%n", e.getMessage());
            return;
        }
        System.out.printf("%n使用BufferedReader + FileReader逐行读取:%n%n");
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            int lineNum = 0;
            while ((line = reader.readLine()) != null) {
                lineNum++;
                System.out.printf("第%d行: %s%n", lineNum, line);
            }
        } catch (IOException e) {
            System.out.printf("读取文件失败: %s%n", e.getMessage());
            return;
        }
        System.out.printf("%n共计读取 %d 行文本。%n",
                countLines(filePath));
    }

    private static int countLines(String filePath) {
        int count = 0;
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            while (reader.readLine() != null) {
                count++;
            }
        } catch (IOException e) {
            System.out.printf("统计行数失败: %s%n", e.getMessage());
        }
        return count;
    }
}
