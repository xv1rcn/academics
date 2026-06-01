import java.io.*;
import java.util.regex.*;
import java.util.LinkedHashMap;
import java.util.Map;

public class ChineseCharCount {
    public static void main(String[] args) {
        String resDir = "build";
        File dir = new File(resDir);
        if (!dir.exists()) {
            dir.mkdirs();
        }

        String dataPath = resDir + File.separator + "data.txt";
        String resultPath = resDir + File.separator + "result.txt";

        String text = "计算机计算真的很快";

        try (PrintWriter writer = new PrintWriter(new FileWriter(dataPath))) {
            writer.print(text);
        } catch (IOException e) {
            System.out.printf("创建build/data.txt失败: %s%n", e.getMessage());
            return;
        }

        System.out.printf("原始文本 (build/data.txt):%n%s%n%n", text);

        Pattern pattern = Pattern.compile("[\\u4e00-\\u9fa5]");
        Matcher matcher = pattern.matcher(text);

        LinkedHashMap<String, Integer> freqMap = new LinkedHashMap<>();

        while (matcher.find()) {
            String ch = matcher.group();
            freqMap.put(ch, freqMap.getOrDefault(ch, 0) + 1);
        }

        StringBuilder result = new StringBuilder();
        for (Map.Entry<String, Integer> entry : freqMap.entrySet()) {
            result.append(String.format("(%s,%d)", entry.getKey(), entry.getValue()));
        }

        try (PrintWriter writer = new PrintWriter(new FileWriter(resultPath))) {
            writer.print(result.toString());
        } catch (IOException e) {
            System.out.printf("写入build/result.txt失败: %s%n", e.getMessage());
            return;
        }

        System.out.printf("汉字频次统计结果 (build/result.txt):%n%s%n", result.toString());
    }
}
