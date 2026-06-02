import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class PreparedQuery {

    private static final String URL = "jdbc:h2:mem:testdb;DB_CLOSE_DELAY=-1";
    private static final String USER = "sa";
    private static final String PASS = "";

    public List<Student> findStudentsByAge(Connection conn, int minAge, int maxAge) throws SQLException {
        List<Student> list = new ArrayList<>();
        String sql = "SELECT id, name, age FROM students WHERE age BETWEEN ? AND ?";
        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, minAge);
            ps.setInt(2, maxAge);
            try (ResultSet rs = ps.executeQuery()) {
                while (rs.next()) {
                    list.add(new Student(rs.getInt("id"), rs.getString("name"), rs.getInt("age")));
                }
            }
        }
        return list;
    }

    public List<Student> unsafeFindStudentsByAge(Connection conn, String minAge, String maxAge) throws SQLException {
        List<Student> list = new ArrayList<>();
        String sql = "SELECT id, name, age FROM students WHERE age BETWEEN " + minAge + " AND " + maxAge;
        try (Statement stmt = conn.createStatement()) {
            try (ResultSet rs = stmt.executeQuery(sql)) {
                while (rs.next()) {
                    list.add(new Student(rs.getInt("id"), rs.getString("name"), rs.getInt("age")));
                }
            }
        }
        return list;
    }

    public static void main(String[] args) {
        try {
            Class.forName("org.h2.Driver");
        } catch (ClassNotFoundException e) {
            System.out.printf("H2驱动加载失败: %s%n", e.getMessage());
            return;
        }

        try (Connection conn = DriverManager.getConnection(URL, USER, PASS)) {

            try (Statement stmt = conn.createStatement()) {
                stmt.execute("CREATE TABLE students (id INT PRIMARY KEY, name VARCHAR(50), age INT)");
            }

            try (PreparedStatement ps = conn.prepareStatement("INSERT INTO students VALUES (?, ?, ?)")) {
                ps.setInt(1, 1);
                ps.setString(2, "张三");
                ps.setInt(3, 16);
                ps.executeUpdate();
                ps.setInt(1, 2);
                ps.setString(2, "李四");
                ps.setInt(3, 19);
                ps.executeUpdate();
                ps.setInt(1, 3);
                ps.setString(2, "王五");
                ps.setInt(3, 21);
                ps.executeUpdate();
                ps.setInt(1, 4);
                ps.setString(2, "赵六");
                ps.setInt(3, 24);
                ps.executeUpdate();
                ps.setInt(1, 5);
                ps.setString(2, "孙七");
                ps.setInt(3, 28);
                ps.executeUpdate();
            }

            PreparedQuery pq = new PreparedQuery();

            System.out.printf("=== 使用PreparedStatement安全查询 ===%n");
            System.out.printf("查询年龄在20到25之间的学生:%n");
            List<Student> result = pq.findStudentsByAge(conn, 20, 25);
            for (Student s : result) {
                System.out.printf("  %s%n", s);
            }

            System.out.printf("%n=== SQL注入测试(不安全方式 - Statement拼接) ===%n");
            System.out.printf("尝试注入 \"20 OR 1=1\" 作为参数:%n");
            List<Student> unsafeResult = pq.unsafeFindStudentsByAge(conn, "20", "25 OR 1=1");
            for (Student s : unsafeResult) {
                System.out.printf("  %s%n", s);
            }
            System.out.printf("警告: Statement拼接方式返回了所有学生,查询条件被绕过!%n");

            System.out.printf("%n=== SQL注入测试(安全方式 - PreparedStatement) ===%n");
            System.out.printf("PreparedStatement使用参数化查询,将输入作为数值处理:%n");
            System.out.printf("调用 findStudentsByAge(20, 25) 时,?占位符只能接收数值,%n");
            System.out.printf("无法注入SQL代码,查询结果严格限制在20到25岁之间.%n");
            result = pq.findStudentsByAge(conn, 20, 25);
            for (Student s : result) {
                System.out.printf("  %s%n", s);
            }
            System.out.printf("PreparedStatement有效防止了SQL注入攻击!%n");

        } catch (SQLException e) {
            System.out.printf("数据库操作异常: %s%n", e.getMessage());
        }
    }
}
