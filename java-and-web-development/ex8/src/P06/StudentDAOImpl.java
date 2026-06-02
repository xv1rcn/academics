import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class StudentDAOImpl implements StudentDAO {

    public StudentDAOImpl() {
        Connection conn = SimpleConnectionPool.getConnection();
        try {
            Statement stmt = conn.createStatement();
            stmt.execute("CREATE TABLE IF NOT EXISTS student (id INT PRIMARY KEY, name VARCHAR(50), age INT)");
            stmt.close();
        } catch (SQLException e) {
            System.err.printf("建表失败: %s%n", e.getMessage());
        } finally {
            SimpleConnectionPool.releaseConnection(conn);
        }
    }

    @Override
    public int insert(Student student) {
        Connection conn = SimpleConnectionPool.getConnection();
        try {
            PreparedStatement ps = conn.prepareStatement("INSERT INTO student (id, name, age) VALUES (?, ?, ?)");
            ps.setInt(1, student.getId());
            ps.setString(2, student.getName());
            ps.setInt(3, student.getAge());
            int result = ps.executeUpdate();
            ps.close();
            return result;
        } catch (SQLException e) {
            System.err.printf("插入失败: %s%n", e.getMessage());
            return 0;
        } finally {
            SimpleConnectionPool.releaseConnection(conn);
        }
    }

    @Override
    public int update(Student student) {
        Connection conn = SimpleConnectionPool.getConnection();
        try {
            PreparedStatement ps = conn.prepareStatement("UPDATE student SET name = ?, age = ? WHERE id = ?");
            ps.setString(1, student.getName());
            ps.setInt(2, student.getAge());
            ps.setInt(3, student.getId());
            int result = ps.executeUpdate();
            ps.close();
            return result;
        } catch (SQLException e) {
            System.err.printf("更新失败: %s%n", e.getMessage());
            return 0;
        } finally {
            SimpleConnectionPool.releaseConnection(conn);
        }
    }

    @Override
    public int delete(int id) {
        Connection conn = SimpleConnectionPool.getConnection();
        try {
            PreparedStatement ps = conn.prepareStatement("DELETE FROM student WHERE id = ?");
            ps.setInt(1, id);
            int result = ps.executeUpdate();
            ps.close();
            return result;
        } catch (SQLException e) {
            System.err.printf("删除失败: %s%n", e.getMessage());
            return 0;
        } finally {
            SimpleConnectionPool.releaseConnection(conn);
        }
    }

    @Override
    public Student findById(int id) {
        Connection conn = SimpleConnectionPool.getConnection();
        try {
            PreparedStatement ps = conn.prepareStatement("SELECT * FROM student WHERE id = ?");
            ps.setInt(1, id);
            ResultSet rs = ps.executeQuery();
            Student student = null;
            if (rs.next()) {
                student = new Student(rs.getInt("id"), rs.getString("name"), rs.getInt("age"));
            }
            rs.close();
            ps.close();
            return student;
        } catch (SQLException e) {
            System.err.printf("查询失败: %s%n", e.getMessage());
            return null;
        } finally {
            SimpleConnectionPool.releaseConnection(conn);
        }
    }

    @Override
    public List<Student> findAll() {
        Connection conn = SimpleConnectionPool.getConnection();
        try {
            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery("SELECT * FROM student");
            List<Student> list = new ArrayList<>();
            while (rs.next()) {
                list.add(new Student(rs.getInt("id"), rs.getString("name"), rs.getInt("age")));
            }
            rs.close();
            stmt.close();
            return list;
        } catch (SQLException e) {
            System.err.printf("查询全部失败: %s%n", e.getMessage());
            return new ArrayList<>();
        } finally {
            SimpleConnectionPool.releaseConnection(conn);
        }
    }
}
