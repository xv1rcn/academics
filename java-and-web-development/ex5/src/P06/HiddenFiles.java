import java.io.File;
import java.io.IOException;

public class HiddenFiles {
    public static void main(String[] args) {
        createTestFiles();
        listHiddenFiles();
    }

    private static void listHiddenFiles() {
        File dir = new File("build");
        File[] files = dir.listFiles();
        if (files == null) {
            System.out.printf("目录不存在或无法读取%n");
            return;
        }
        int count = 0;
        for (File f : files) {
            if (f.isHidden()) {
                count++;
                System.out.printf("隐藏%s: %s%n", f.isDirectory() ? "文件夹" : "文件", f.getName());
            }
        }
        if (count == 0) {
            System.out.printf("当前目录下没有隐藏文件或文件夹%n");
        } else {
            System.out.printf("共找到 %d 个隐藏文件/文件夹%n", count);
        }
    }

    private static void createTestFiles() {
        File resDir = new File("build");
        if (!resDir.exists()) {
            resDir.mkdirs();
        }
        String[] hiddenFiles = {".hidden_config", ".secret_data", ".tmp_cache", ".ignored_dir"};
        String[] hiddenDirs = {".hidden_folder", ".secret_dir"};
        for (String name : hiddenFiles) {
            File f = new File(resDir, name);
            if (!f.exists()) {
                try {
                    f.createNewFile();
                } catch (IOException e) {
                    System.out.printf("创建文件失败: %s%n", name);
                }
            }
        }
        for (String name : hiddenDirs) {
            File d = new File(resDir, name);
            if (!d.exists()) {
                d.mkdir();
            }
        }
        String[] normalFiles = {"visible.txt", "data.csv", "score.txt"};
        for (String name : normalFiles) {
            File f = new File(resDir, name);
            if (!f.exists()) {
                try {
                    f.createNewFile();
                } catch (IOException e) {
                    System.out.printf("创建文件失败: %s%n", name);
                }
            }
        }
        String[] normalDirs = {"public_data", "logs"};
        for (String name : normalDirs) {
            File d = new File(resDir, name);
            if (!d.exists()) {
                d.mkdir();
            }
        }
    }
}
