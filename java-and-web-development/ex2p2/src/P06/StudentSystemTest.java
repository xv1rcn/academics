import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

public class StudentSystemTest {
    public static void main(String[] args) {
        List<Student> list = new ArrayList<>();
        list.add(new Student("张三", "S01", 80, 90, 70));
        list.add(new Student("李四", "S02", 85, 95, 60));
        list.add(new Student("王五", "S03", 75, 85, 95));

        Statistics stats = new SimpleStatistics();

        System.out.println("自然排序, 总分降序: ");
        Collections.sort(list);
        list.forEach(System.out::println);

        System.out.println("按姓名升序:");
        Collections.sort(list, new ByNameComparator());
        list.forEach(System.out::println);

        System.out.println("按数学成绩降序:");
        Collections.sort(list, new ByMathScoreComparator());
        list.forEach(System.out::println);

        System.out.println("按总分升序:");
        Collections.sort(list, new ByTotalScoreComparator());
        list.forEach(System.out::println);

        System.out.println("统计结果:");
        System.out.println("全班平均分：" + stats.averageScore(list));
        System.out.println("总分最高：" + stats.getHighestTotalStudent(list).getName());

        Map<String, Double> subAvg = stats.getSubjectAverage(list);
        System.out.println("各科平均分：" + subAvg);
    }
}
