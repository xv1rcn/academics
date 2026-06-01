import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class DefaultCharsetFile {
    @Override
    public String toString() {
        return "DefaultCharsetFile";
    }

    public static void main(String[] args) {
        new File("build").mkdirs();
        String filePath = "build/charset_test.txt";
        String content = "计算机真的很快";
        try {
            FileWriter writer = new FileWriter(filePath);
            writer.write(content);
            writer.close();
            System.out.printf("已创建文件 %s ，写入内容：%s%n", filePath, content);
        } catch (IOException e) {
            System.out.printf("写入文件失败：%s%n", e.getMessage());
            return;
        }
        try {
            FileReader reader = new FileReader(filePath);
            System.out.printf("使用FileReader默认编码逐字符读取 %s ：%n", filePath);
            int ch;
            while ((ch = reader.read()) != -1) {
                System.out.printf("%c", (char) ch);
            }
            System.out.printf("%n读取完成%n");
            reader.close();
        } catch (IOException e) {
            System.out.printf("读取文件失败：%s%n", e.getMessage());
        }
    }
}
