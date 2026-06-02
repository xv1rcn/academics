import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.Statement;

public class BatchInsert {
    public static void main(String[] args) throws Exception {
        Class.forName("org.h2.Driver");
        Connection conn = DriverManager.getConnection("jdbc:h2:mem:batchdb");
        conn.setAutoCommit(false);

        Statement stmt = conn.createStatement();
        stmt.executeUpdate("CREATE TABLE students (id INT PRIMARY KEY, name VARCHAR(50), score DOUBLE)");
        stmt.close();

        long batchTime = batchInsert(conn);

        stmt = conn.createStatement();
        stmt.executeUpdate("DROP TABLE students");
        stmt.executeUpdate("CREATE TABLE students (id INT PRIMARY KEY, name VARCHAR(50), score DOUBLE)");
        stmt.close();

        long singleTime = singleInsert(conn);

        conn.close();

        System.out.printf("批处理耗时: %dms%n逐条插入耗时: %dms%n", batchTime, singleTime);
    }

    static long batchInsert(Connection conn) throws Exception {
        String sql = "INSERT INTO students (id, name, score) VALUES (?, ?, ?)";
        PreparedStatement pstmt = conn.prepareStatement(sql);
        long start = System.currentTimeMillis();
        for (int i = 1; i <= 10000; i++) {
            pstmt.setInt(1, i);
            pstmt.setString(2, "学生" + i);
            pstmt.setDouble(3, Math.random() * 100);
            pstmt.addBatch();
            if (i % 1000 == 0) {
                pstmt.executeBatch();
                conn.commit();
            }
        }
        pstmt.close();
        long end = System.currentTimeMillis();
        return end - start;
    }

    static long singleInsert(Connection conn) throws Exception {
        String sql = "INSERT INTO students (id, name, score) VALUES (?, ?, ?)";
        PreparedStatement pstmt = conn.prepareStatement(sql);
        long start = System.currentTimeMillis();
        for (int i = 1; i <= 10000; i++) {
            pstmt.setInt(1, i);
            pstmt.setString(2, "学生" + i);
            pstmt.setDouble(3, Math.random() * 100);
            pstmt.executeUpdate();
            conn.commit();
        }
        pstmt.close();
        long end = System.currentTimeMillis();
        return end - start;
    }
}
