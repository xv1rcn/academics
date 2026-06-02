import java.sql.*;
import java.util.*;

public class PageQuery {

    private static final String DB_URL = "jdbc:h2:mem:pagedb;DB_CLOSE_DELAY=-1";

    static class Student {
        private int id;
        private String name;

        Student(int id, String name) {
            this.id = id;
            this.name = name;
        }

        public int getId() {
            return id;
        }

        public String getName() {
            return name;
        }

        @Override
        public String toString() {
            return String.format("ID: %d, 姓名: %s", id, name);
        }
    }

    public static PageResult<Student> getStudentsByPage(int pageNo, int pageSize) {
        try (Connection conn = DriverManager.getConnection(DB_URL)) {
            int totalRecords;
            try (Statement stmt = conn.createStatement();
                 ResultSet rs = stmt.executeQuery("SELECT COUNT(*) FROM students")) {
                rs.next();
                totalRecords = rs.getInt(1);
            }
            int totalPages = (int) Math.ceil((double) totalRecords / pageSize);
            int offset = (pageNo - 1) * pageSize;
            List<Student> data = new ArrayList<>();
            try (PreparedStatement pstmt = conn.prepareStatement(
                    "SELECT id, name FROM students ORDER BY id LIMIT ? OFFSET ?")) {
                pstmt.setInt(1, pageSize);
                pstmt.setInt(2, offset);
                try (ResultSet rs = pstmt.executeQuery()) {
                    while (rs.next()) {
                        data.add(new Student(rs.getInt("id"), rs.getString("name")));
                    }
                }
            }
            return new PageResult<>(data, totalRecords, totalPages, pageNo);
        } catch (SQLException e) {
            System.out.printf("数据库异常: %s%n", e.getMessage());
            return new PageResult<>(new ArrayList<>(), 0, 0, pageNo);
        }
    }

    public static void main(String[] args) throws Exception {
        Class.forName("org.h2.Driver");

        try (Connection conn = DriverManager.getConnection(DB_URL);
             Statement stmt = conn.createStatement()) {
            stmt.execute("CREATE TABLE students (id INT PRIMARY KEY, name VARCHAR(50))");
            for (int i = 1; i <= 25; i++) {
                stmt.execute(String.format("INSERT INTO students VALUES (%d, '学生%d')", i, i));
            }
        }

        int pageSize = 5;

        System.out.printf("========== 分页查询演示 ==========%n%n");

        PageResult<Student> page2 = getStudentsByPage(2, pageSize);
        System.out.printf("第%d页  (总页数: %d, 总记录数: %d)%n",
                page2.getCurrentPage(), page2.getTotalPages(), page2.getTotalRecords());
        for (Student s : page2.getData()) {
            System.out.printf("  %s%n", s);
        }
        System.out.printf("%n");

        PageResult<Student> page3 = getStudentsByPage(3, pageSize);
        System.out.printf("第%d页  (总页数: %d, 总记录数: %d)%n",
                page3.getCurrentPage(), page3.getTotalPages(), page3.getTotalRecords());
        for (Student s : page3.getData()) {
            System.out.printf("  %s%n", s);
        }
    }
}
