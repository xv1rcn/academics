import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.File;
import java.io.IOException;
import java.util.Random;

public class RandomIntFile {

    public static void main(String[] args) {
        String filePath = "build/data.dat";
        int count = 100;
        Random rand = new Random();

        new File("build").mkdirs();

        try (DataOutputStream dos = new DataOutputStream(new FileOutputStream(filePath))) {
            for (int i = 0; i < count; i++) {
                dos.writeInt(rand.nextInt());
            }
            System.out.printf("已写入 %d 个随机整数到文件 %s%n", count, filePath);
        } catch (IOException e) {
            System.out.printf("写入文件时发生错误: %s%n", e.getMessage());
            return;
        }

        int sum = 0;
        int[] values = new int[count];

        try (DataInputStream dis = new DataInputStream(new FileInputStream(filePath))) {
            System.out.printf("%n从文件 %s 读取数据:%n%n", filePath);
            for (int i = 0; i < count; i++) {
                values[i] = dis.readInt();
                sum += values[i];
                System.out.printf("整数[%d] = %d%n", i + 1, values[i]);
            }
        } catch (IOException e) {
            System.out.printf("读取文件时发生错误: %s%n", e.getMessage());
            return;
        }

        double average = (double) sum / count;

        System.out.printf("%n--- 统计结果 ---%n");
        System.out.printf("总和 = %d%n", sum);
        System.out.printf("平均值 = %.2f%n", average);
    }
}
