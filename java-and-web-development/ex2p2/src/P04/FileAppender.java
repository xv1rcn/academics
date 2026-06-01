import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class FileAppender implements LogAppender {
    private final String filePath;

    public FileAppender(String filePath) {
        this.filePath = filePath;
    }

    @Override
    public void append(String message) {
        try (PrintWriter pw = new PrintWriter(new FileWriter(filePath, true))) {
            pw.println(message);
            System.out.println("已写入文件: " + message);
        } catch (IOException e) {
            System.err.println("文件写入失败: " + e.getMessage());
        }
    }
}
