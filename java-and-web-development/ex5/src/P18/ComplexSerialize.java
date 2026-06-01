import java.io.*;
import java.util.Random;

public class ComplexSerialize {
    public static void main(String[] args) {
        String filePath = "build/complex.ser";
        new java.io.File("build").mkdirs();
        Random rand = new Random();
        int count = 10;

        Complex[] complexes = new Complex[count];
        for (int i = 0; i < count; i++) {
            double real = Math.round(rand.nextDouble() * 200 - 100) / 10.0;
            double imag = Math.round(rand.nextDouble() * 200 - 100) / 10.0;
            complexes[i] = new Complex(real, imag);
        }

        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(filePath))) {
            oos.writeObject(complexes);
            System.out.printf("序列化完成：已将%d个复数对象写入%s%n", count, filePath);
        } catch (IOException e) {
            System.out.printf("序列化失败：%s%n", e.getMessage());
            return;
        }

        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(filePath))) {
            Complex[] loaded = (Complex[]) ois.readObject();
            System.out.printf("反序列化读取结果（共%d个复数对象）：%n", loaded.length);
            for (int i = 0; i < loaded.length; i++) {
                System.out.printf("  [%d] %s%n", i + 1, loaded[i]);
            }
        } catch (IOException | ClassNotFoundException e) {
            System.out.printf("反序列化失败：%s%n", e.getMessage());
        }
    }
}
