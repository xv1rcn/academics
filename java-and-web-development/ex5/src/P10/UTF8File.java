import java.io.OutputStreamWriter;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.FileInputStream;
import java.io.IOException;

public class UTF8File {
    public static void main(String[] args) {
        String filePath = "build/utf8_test.txt";
        new java.io.File("build").mkdirs();
        writeFile(filePath);
        readFile(filePath);
    }

    private static void writeFile(String filePath) {
        try (OutputStreamWriter writer = new OutputStreamWriter(
                new FileOutputStream(filePath), "UTF-8")) {
            writer.write("你好世界！这是一个UTF-8编码的中文测试文件。\n");
            writer.write("Java IO 流操作示例：OutputStreamWriter 写入中文。\n");
            writer.write("阳光明媚，春意盎然，编程学习正当时。\n");
            System.out.printf("文件写入成功：%s%n", filePath);
        } catch (IOException e) {
            System.out.printf("写入文件时发生错误：%s%n", e.getMessage());
        }
    }

    private static void readFile(String filePath) {
        try (InputStreamReader reader = new InputStreamReader(
                new FileInputStream(filePath), "UTF-8")) {
            System.out.printf("%n读取文件内容：%n");
            System.out.printf("----------------------------------------%n");
            int ch;
            while ((ch = reader.read()) != -1) {
                System.out.printf("%c", ch);
            }
            System.out.printf("----------------------------------------%n");
            System.out.printf("%n文件读取完毕。%n");
        } catch (IOException e) {
            System.out.printf("读取文件时发生错误：%s%n", e.getMessage());
        }
    }
}
