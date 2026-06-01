import java.io.*;
import java.util.regex.*;
import java.util.ArrayList;
import java.util.List;

public class ExtractNumbers {
    public static void main(String[] args) {
        String resDir = "build";
        File dir = new File(resDir);
        if (!dir.exists()) {
            dir.mkdirs();
        }

        String t1Path = resDir + File.separator + "t1.txt";
        String t2Path = resDir + File.separator + "t2.txt";

        String originalText = "abc123def456ghi789xyz012hello345world678java901test234code567end890";

        try (PrintWriter writer = new PrintWriter(new FileWriter(t1Path))) {
            writer.print(originalText);
        } catch (IOException e) {
            System.out.printf("创建t1.txt失败: %s%n", e.getMessage());
            return;
        }

        System.out.printf("原始文本 (build/t1.txt):%n%s%n%n", originalText);

        Pattern pattern = Pattern.compile("\\d+");
        Matcher matcher = pattern.matcher(originalText);

        List<String> numbers = new ArrayList<>();
        while (matcher.find()) {
            numbers.add(matcher.group());
        }

        StringBuilder extracted = new StringBuilder();
        for (int i = 0; i < numbers.size(); i++) {
            if (i > 0) {
                extracted.append(" ");
            }
            extracted.append(numbers.get(i));
        }

        try (PrintWriter writer = new PrintWriter(new FileWriter(t2Path))) {
            writer.print(extracted.toString());
        } catch (IOException e) {
            System.out.printf("写入t2.txt失败: %s%n", e.getMessage());
            return;
        }

        System.out.printf("提取的数字 (build/t2.txt):%n%s%n", extracted.toString());
    }
}
