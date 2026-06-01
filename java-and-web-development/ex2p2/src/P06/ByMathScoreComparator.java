import java.util.Comparator;

public class ByMathScoreComparator implements Comparator<Student> {
    @Override
    public int compare(Student s1, Student s2) {
        return Double.compare(s2.getMath(), s1.getMath());
    }
}
