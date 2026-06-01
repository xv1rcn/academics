import java.io.File;
import java.io.IOException;

public class FolderStats {
    private int folderCount;
    private int fileCount;
    private long totalSize;

    public static void main(String[] args) {
        new FolderStats().run();
    }

    public void run() {
        File buildDir = new File("build");
        if (!buildDir.exists()) {
            buildDir.mkdirs();
        }

        try {
            new File("build/data1.txt").createNewFile();
            new File("build/data2.txt").createNewFile();
            File subDir = new File("build/sub");
            subDir.mkdirs();
            new File("build/sub/nested.txt").createNewFile();

            java.io.FileWriter fw = new java.io.FileWriter("build/data1.txt");
            fw.write("Test data content for file 1.");
            fw.close();
            fw = new java.io.FileWriter("build/data2.txt");
            fw.write("Another test data file with more content.");
            fw.close();
            fw = new java.io.FileWriter("build/sub/nested.txt");
            fw.write("Nested file content data.");
            fw.close();
        } catch (IOException e) {
            System.err.println("Error creating test files: " + e.getMessage());
        }

        File startDir = buildDir.getAbsoluteFile();
        traverse(startDir);
        System.out.printf("子文件夹总数: %d%n", folderCount);
        System.out.printf("文件总数: %d%n", fileCount);
        System.out.printf("总空间大小: %d 字节%n", totalSize);
    }

    @Override
    public String toString() {
        return String.format("FolderStats[文件夹:%d, 文件:%d, 大小:%d字节]",
                folderCount, fileCount, totalSize);
    }

    private void traverse(File dir) {
        File[] entries = dir.listFiles();
        if (entries == null) {
            return;
        }
        for (File entry : entries) {
            if (entry.isDirectory()) {
                folderCount++;
                traverse(entry);
            } else {
                fileCount++;
                totalSize += entry.length();
            }
        }
    }
}
