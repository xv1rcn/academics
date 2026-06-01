import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class StudentSort {
    public static void main(String[] args) {
        List<Student> students = new ArrayList<>();
        students.add(new Student(103, "张三", 88.5));
        students.add(new Student(101, "李四", 92.0));
        students.add(new Student(104, "王五", 76.5));
        students.add(new Student(102, "赵六", 88.5));
        students.add(new Student(105, "孙七", 92.0));

        List<Student> list1 = new ArrayList<>(students);
        Collections.sort(list1, new Comparator<Student>() {
            @Override
            public int compare(Student s1, Student s2) {
                if (s2.getScore() != s1.getScore()) {
                    return Double.compare(s2.getScore(), s1.getScore());
                }
                return Integer.compare(s1.getId(), s2.getId());
            }
        });

        System.out.printf("使用Comparator方式排序结果:%n");
        for (Student s : list1) {
            System.out.printf("%s%n", s);
        }

        List<Student> list2 = new ArrayList<>(students);
        Collections.sort(list2, (s1, s2) -> {
            if (s2.getScore() != s1.getScore()) {
                return Double.compare(s2.getScore(), s1.getScore());
            }
            return Integer.compare(s1.getId(), s2.getId());
        });

        System.out.printf("%n使用Lambda方式排序结果:%n");
        for (Student s : list2) {
            System.out.printf("%s%n", s);
        }
    }
}
