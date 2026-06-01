import java.io.File;
import java.io.FilenameFilter;
import java.io.FileWriter;
import java.io.IOException;

public class DocFiles {
    public static void main(String[] args) {
        File dir = new File("build");

        if (!dir.exists()) {
            dir.mkdirs();
        }

        if (!dir.isDirectory()) {
            System.out.printf("build/ exists but is not a directory%n");
            return;
        }

        String[] testFiles = {"summary.doc", "report.doc", "data.doc", "notes.doc", "backup.doc"};
        for (String name : testFiles) {
            try {
                File f = new File(dir, name);
                if (!f.exists()) {
                    new FileWriter(f).close();
                }
            } catch (IOException e) {
                System.out.printf("Failed to create %s: %s%n", name, e.getMessage());
            }
        }

        FilenameFilter docFilter = new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                return name.toLowerCase().endsWith(".doc");
            }
        };

        String[] docFiles = dir.list(docFilter);
        if (docFiles == null || docFiles.length == 0) {
            System.out.printf("未找到 .doc 文件%n");
            return;
        }

        System.out.printf("找到 %d 个 .doc 文件：%n", docFiles.length);
        for (int i = 0; i < docFiles.length; i++) {
            System.out.printf("  %s%n", docFiles[i]);
        }
    }
}
