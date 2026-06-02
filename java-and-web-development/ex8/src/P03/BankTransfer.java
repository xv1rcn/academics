import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class BankTransfer {
    private static final String URL = "jdbc:h2:mem:bank;DB_CLOSE_DELAY=-1";
    private static final String USER = "sa";
    private static final String PASS = "";

    public static void main(String[] args) {
        BankTransfer bt = new BankTransfer();
        bt.initDB();
        System.out.printf("初始余额:%n");
        bt.printBalances();

        System.out.printf("%n① 成功转账: A→B 200元%n");
        bt.transfer(1, 2, 200);
        bt.printBalances();

        System.out.printf("%n② 余额不足回滚: A→B 900元%n");
        bt.transfer(1, 2, 900);
        bt.printBalances();
    }

    private Connection getConnection() throws SQLException {
        return DriverManager.getConnection(URL, USER, PASS);
    }

    private void initDB() {
        try (Connection conn = getConnection(); Statement stmt = conn.createStatement()) {
            stmt.execute("CREATE TABLE account(id INT PRIMARY KEY, name VARCHAR(255), balance DOUBLE)");
            stmt.execute("INSERT INTO account VALUES(1, '张三', 1000)");
            stmt.execute("INSERT INTO account VALUES(2, '李四', 500)");
        } catch (SQLException e) {
            System.out.printf("数据库初始化失败: %s%n", e.getMessage());
        }
    }

    public void transfer(int fromId, int toId, double amount) {
        Connection conn = null;
        try {
            conn = getConnection();
            conn.setAutoCommit(false);
            double balance = getBalance(conn, fromId);
            if (balance < amount) {
                throw new RuntimeException("余额不足, 当前余额: " + balance + "元, 需要: " + amount + "元");
            }
            String deductSql = "UPDATE account SET balance = balance - ? WHERE id = ?";
            try (PreparedStatement ps = conn.prepareStatement(deductSql)) {
                ps.setDouble(1, amount);
                ps.setInt(2, fromId);
                ps.executeUpdate();
            }
            String addSql = "UPDATE account SET balance = balance + ? WHERE id = ?";
            try (PreparedStatement ps = conn.prepareStatement(addSql)) {
                ps.setDouble(1, amount);
                ps.setInt(2, toId);
                ps.executeUpdate();
            }
            conn.commit();
            System.out.printf("转账成功: 账户%d → 账户%d, 金额%.2f元%n", fromId, toId, amount);
        } catch (Exception e) {
            if (conn != null) {
                try {
                    conn.rollback();
                    System.out.printf("事务已回滚%n");
                } catch (SQLException ex) {
                    System.out.printf("回滚异常: %s%n", ex.getMessage());
                }
            }
            System.out.printf("转账失败: %s%n", e.getMessage());
        } finally {
            if (conn != null) {
                try {
                    conn.close();
                } catch (SQLException e) {
                    System.out.printf("关闭连接异常: %s%n", e.getMessage());
                }
            }
        }
    }

    private double getBalance(Connection conn, int id) throws SQLException {
        String sql = "SELECT balance FROM account WHERE id = ?";
        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, id);
            try (ResultSet rs = ps.executeQuery()) {
                if (rs.next()) {
                    return rs.getDouble("balance");
                }
            }
        }
        throw new RuntimeException("账户" + id + "不存在");
    }

    private void printBalances() {
        try (Connection conn = getConnection(); Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT * FROM account")) {
            while (rs.next()) {
                System.out.printf("账户%d %s 余额: %.2f元%n",
                    rs.getInt("id"), rs.getString("name"), rs.getDouble("balance"));
            }
        } catch (SQLException e) {
            System.out.printf("查询余额失败: %s%n", e.getMessage());
        }
    }
}
