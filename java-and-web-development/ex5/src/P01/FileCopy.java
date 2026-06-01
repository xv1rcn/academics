import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class FileCopy {
    private static final int BUFFER_SIZE = 2 * 1024 * 1024;

    public static void copyFile(String src, String dest) {
        File srcFile = new File(src);
        File destFile = new File(dest);
        try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(srcFile), BUFFER_SIZE);
             BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(destFile), BUFFER_SIZE)) {
            byte[] buffer = new byte[BUFFER_SIZE];
            int bytesRead;
            while ((bytesRead = bis.read(buffer)) != -1) {
                bos.write(buffer, 0, bytesRead);
            }
            bos.flush();
        } catch (IOException e) {
            System.out.printf("复制文件时发生错误: %s%n", e.getMessage());
        }
    }

    public static void main(String[] args) {
        String srcPath = "build/source.dat";
        String destPath = "build/copy.dat";

        File srcFile = new File(srcPath);
        File parentDir = srcFile.getParentFile();
        if (parentDir != null && !parentDir.exists()) {
            parentDir.mkdirs();
        }

        byte[] testData = new byte[5 * 1024 * 1024];
        for (int i = 0; i < testData.length; i++) {
            testData[i] = (byte) (i % 256);
        }

        try (FileOutputStream fos = new FileOutputStream(srcFile)) {
            fos.write(testData);
            fos.flush();
        } catch (IOException e) {
            System.out.printf("创建源文件时发生错误: %s%n", e.getMessage());
            return;
        }

        copyFile(srcPath, destPath);

        System.out.printf("已成功复制完成%n");
    }
}
