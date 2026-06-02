import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.LinkedList;
import java.util.Queue;

public class SimpleConnectionPool {
    private static final String URL = "jdbc:h2:mem:studentdb";
    private static final String USER = "sa";
    private static final String PASSWORD = "";
    private static final int POOL_SIZE = 5;
    private static final Queue<Connection> pool = new LinkedList<>();

    static {
        try {
            Class.forName("org.h2.Driver");
            for (int i = 0; i < POOL_SIZE; i++) {
                pool.add(DriverManager.getConnection(URL, USER, PASSWORD));
            }
        } catch (Exception e) {
            System.err.printf("连接池初始化失败: %s%n", e.getMessage());
        }
    }

    public static synchronized Connection getConnection() {
        while (pool.isEmpty()) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                System.err.printf("等待连接时被中断: %s%n", e.getMessage());
            }
        }
        return pool.poll();
    }

    public static synchronized void releaseConnection(Connection conn) {
        if (conn != null) {
            pool.add(conn);
        }
    }

    public static void shutdown() {
        for (Connection conn : pool) {
            try {
                conn.close();
            } catch (SQLException e) {
                System.err.printf("关闭连接失败: %s%n", e.getMessage());
            }
        }
        pool.clear();
    }
}
