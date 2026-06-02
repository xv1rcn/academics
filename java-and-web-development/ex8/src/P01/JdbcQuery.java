import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

public class JdbcQuery {
    public static void main(String[] args) throws Exception {
        Class.forName("org.h2.Driver");
        Connection conn = null;
        Statement stmt = null;
        ResultSet rs = null;
        try {
            conn = DriverManager.getConnection("jdbc:h2:mem:testdb;DB_CLOSE_DELAY=-1");
            stmt = conn.createStatement();
            stmt.execute("CREATE TABLE student(id INT PRIMARY KEY, name VARCHAR, age INT)");
            stmt.execute("INSERT INTO student VALUES(1, '张三', 20)");
            stmt.execute("INSERT INTO student VALUES(2, '李四', 22)");
            stmt.execute("INSERT INTO student VALUES(3, '王五', 19)");
            rs = stmt.executeQuery("SELECT * FROM student");
            while (rs.next()) {
                System.out.printf("学号:%d 姓名:%s 年龄:%d%n", rs.getInt("id"), rs.getString("name"), rs.getInt("age"));
            }
        } finally {
            if (rs != null) {
                rs.close();
            }
            if (stmt != null) {
                stmt.close();
            }
            if (conn != null) {
                conn.close();
            }
        }
    }
}
