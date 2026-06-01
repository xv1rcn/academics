import java.io.*;
import java.util.*;

public class MatrixTranspose {
    public static void main(String[] args) {
        String dataFile = "build/data.txt";
        String transFile = "build/data90.txt";

        int rows = 3;
        int cols = 5;

        new File("build").mkdirs();

        int[][] matrix = new int[rows][cols];

        Random rand = new Random();
        try (PrintWriter writer = new PrintWriter(new FileWriter(dataFile))) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    int val = rand.nextInt(100);
                    writer.print(val);
                    if (j < cols - 1) writer.print(" ");
                }
                writer.println();
            }
        } catch (IOException e) {
            System.out.printf("写入文件失败: %s%n", e.getMessage());
            return;
        }

        try (Scanner scanner = new Scanner(new File(dataFile))) {
            int i = 0;
            while (scanner.hasNextLine() && i < rows) {
                String[] tokens = scanner.nextLine().trim().split("\\s+");
                for (int j = 0; j < tokens.length && j < cols; j++) {
                    matrix[i][j] = Integer.parseInt(tokens[j]);
                }
                i++;
            }
        } catch (IOException e) {
            System.out.printf("读取文件失败: %s%n", e.getMessage());
            return;
        }

        System.out.printf("原始矩阵 (3行×5列):%n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                System.out.printf("%4d", matrix[i][j]);
            }
            System.out.printf("%n");
        }

        int[][] transposed = new int[cols][rows];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                transposed[j][i] = matrix[i][j];
            }
        }

        try (PrintWriter writer = new PrintWriter(new FileWriter(transFile))) {
            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {
                    writer.print(transposed[i][j]);
                    if (j < rows - 1) writer.print(" ");
                }
                writer.println();
            }
        } catch (IOException e) {
            System.out.printf("写入转置文件失败: %s%n", e.getMessage());
            return;
        }

        System.out.printf("%n转置后矩阵 (5行×3列):%n");
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                System.out.printf("%4d", transposed[i][j]);
            }
            System.out.printf("%n");
        }
    }

    @Override
    public String toString() {
        return "MatrixTranspose";
    }
}
