import java.util.List;
import java.util.Map;

public interface Statistics {
    double averageScore(List<Student> students);
    Student getHighestTotalStudent(List<Student> students);
    Map<String, Double> getSubjectAverage(List<Student> students);
}
