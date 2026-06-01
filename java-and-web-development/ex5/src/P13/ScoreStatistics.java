import java.io.*;
import java.util.*;

public class ScoreStatistics {
    public static void main(String[] args) {
        ScoreStatistics app = new ScoreStatistics();
        app.generateData();
        app.processScores();
    }

    private void generateData() {
        File dir = new File("build");
        if (!dir.exists()) {
            dir.mkdirs();
        }
        String[][] data = {
            {"2024001", "张三", "88", "92"},
            {"2024002", "李四", "76", "85"},
            {"2024003", "王五", "91", "78"},
            {"2024004", "赵六", "84", "90"},
            {"2024005", "陈七", "95", "88"}
        };
        try (PrintWriter pw = new PrintWriter("build/score.txt")) {
            for (String[] row : data) {
                pw.printf("%s %s %s %s%n", row[0], row[1], row[2], row[3]);
            }
        } catch (FileNotFoundException e) {
            System.out.printf("创建数据文件失败: %s%n", e.getMessage());
        }
    }

    private void processScores() {
        List<String> ids = new ArrayList<>();
        List<String> names = new ArrayList<>();
        List<Double> chineseScores = new ArrayList<>();
        List<Double> mathScores = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader("build/score.txt"))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.trim().split("\\s+");
                if (parts.length < 4) {
                    continue;
                }
                ids.add(parts[0]);
                names.add(parts[1]);
                chineseScores.add(Double.parseDouble(parts[2]));
                mathScores.add(Double.parseDouble(parts[3]));
            }
        } catch (IOException e) {
            System.out.printf("读取数据文件失败: %s%n", e.getMessage());
            return;
        }

        double chineseSum = 0.0;
        double mathSum = 0.0;
        int count = ids.size();

        StringBuilder sb = new StringBuilder();
        sb.append(String.format("==================== 成绩统计 ====================%n"));
        sb.append(String.format("%-8s %-6s %-6s %-6s %-6s%n", "学号", "姓名", "语文", "数学", "总分"));
        sb.append(String.format("----------------------------------------------------%n"));

        for (int i = 0; i < count; i++) {
            double yuwen = chineseScores.get(i);
            double shuxue = mathScores.get(i);
            double total = yuwen + shuxue;
            chineseSum += yuwen;
            mathSum += shuxue;
            sb.append(String.format("%-8s %-6s %-6.1f %-6.1f %-6.1f%n",
                    ids.get(i), names.get(i), yuwen, shuxue, total));
        }

        double chineseAvg = chineseSum / count;
        double mathAvg = mathSum / count;

        sb.append(String.format("----------------------------------------------------%n"));
        sb.append(String.format("语文平均分: %.1f  数学平均分: %.1f%n", chineseAvg, mathAvg));

        String result = sb.toString();
        System.out.print(result);

        try (PrintWriter pw = new PrintWriter("build/result.txt")) {
            pw.print(result);
        } catch (FileNotFoundException e) {
            System.out.printf("写入结果文件失败: %s%n", e.getMessage());
        }
    }
}
