import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SimpleStatistics implements Statistics {
    @Override
    public double averageScore(List<Student> students) {
        if (students.isEmpty()) return 0;
        double sum = 0;
        for (Student s : students) sum += s.getTotal();
        return sum / students.size();
    }

    @Override
    public Student getHighestTotalStudent(List<Student> students) {
        Student top = students.get(0);
        for (Student s : students) {
            if (s.getTotal() > top.getTotal()) top = s;
        }
        return top;
    }

    @Override
    public Map<String, Double> getSubjectAverage(List<Student> students) {
        Map<String, Double> map = new HashMap<>();
        if (students.isEmpty()) return map;

        double ch = 0, ma = 0, en = 0;
        for (Student s : students) {
            ch += s.getChinese();
            ma += s.getMath();
            en += s.getEnglish();
        }
        int n = students.size();
        map.put("语文", ch / n);
        map.put("数学", ma / n);
        map.put("英语", en / n);
        return map;
    }
}
