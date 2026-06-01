import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

public class FolderMove {
    public static void main(String[] args) {
        new File("build").mkdirs();
        File srcDir = new File("build/temp_src");
        if (!srcDir.exists()) {
            srcDir.mkdirs();
        }
        String[] testFiles = {"readme.txt", "data.csv", "notes.md"};
        for (String fileName : testFiles) {
            File f = new File(srcDir, fileName);
            try (FileWriter fw = new FileWriter(f)) {
                fw.write("示例内容: " + fileName);
            } catch (IOException e) {
                System.out.printf("创建文件失败: %s%n", e.getMessage());
            }
        }
        File subDir = new File(srcDir, "sub");
        subDir.mkdir();
        File subFile = new File(subDir, "inner.txt");
        try (FileWriter fw = new FileWriter(subFile)) {
            fw.write("子目录文件内容");
        } catch (IOException e) {
            System.out.printf("创建文件失败: %s%n", e.getMessage());
        }
        File destDir = new File("build/temp_dest");
        deleteDir(destDir);
        try {
            Files.move(srcDir.toPath(), destDir.toPath(), StandardCopyOption.REPLACE_EXISTING);
        } catch (IOException e) {
            System.out.printf("移动失败: %s%n", e.getMessage());
            return;
        }
        System.out.printf("移动完成！%n");
    }

    private static void deleteDir(File dir) {
        if (dir.isDirectory()) {
            File[] children = dir.listFiles();
            if (children != null) {
                for (File child : children) {
                    deleteDir(child);
                }
            }
        }
        dir.delete();
    }
}
