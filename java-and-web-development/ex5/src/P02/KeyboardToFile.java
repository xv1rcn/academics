import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class KeyboardToFile {
    public static void main(String[] args) {
        String simulatedInput = "ABCD";

        File resDir = new File("build");
        if (!resDir.exists()) {
            resDir.mkdirs();
        }

        try (FileOutputStream fos = new FileOutputStream("build/keyboard.dat")) {
            byte[] bytes = simulatedInput.getBytes();
            fos.write(bytes);
            System.out.printf("已将字符串 \"%s\" 的字节数据写入 build/keyboard.dat%n", simulatedInput);
        } catch (IOException e) {
            System.out.printf("写入文件失败: %s%n", e.getMessage());
            return;
        }

        try (FileInputStream fis = new FileInputStream("build/keyboard.dat")) {
            System.out.printf("从 build/keyboard.dat 读取的十六进制字节: ");
            int b;
            boolean first = true;
            while ((b = fis.read()) != -1) {
                if (!first) {
                    System.out.printf(" ");
                }
                System.out.printf("%02X", b);
                first = false;
            }
            System.out.printf("%n");
        } catch (IOException e) {
            System.out.printf("读取文件失败: %s%n", e.getMessage());
        }
    }
}
