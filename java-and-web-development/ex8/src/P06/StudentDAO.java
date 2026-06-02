import java.util.List;

public interface StudentDAO {
    int insert(Student student);
    int update(Student student);
    int delete(int id);
    Student findById(int id);
    List<Student> findAll();
}
