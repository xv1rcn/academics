import java.util.Comparator;

public class ByTotalScoreComparator implements Comparator<Student> {
    @Override
    public int compare(Student s1, Student s2) {
        return Double.compare(s1.getTotal(), s2.getTotal());
    }
}
