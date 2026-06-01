import java.util.Arrays;
import java.util.List;
import java.util.Optional;

public class ListFilter {
    public static void main(String[] args) {
        List<String> fruits = Arrays.asList("apple", "banana", "cherry", "date", "elderberry", "fig", "grape");

        System.out.printf("【任务1】长度>=5的水果：%n");
        fruits.stream()
                .filter(s -> s.length() >= 5)
                .forEach(s -> System.out.printf("  %s%n", s));

        System.out.printf("%n【任务2】第一个以'e'开头的水果：%n");
        Optional<String> firstWithE = fruits.stream()
                .filter(s -> s.startsWith("e"))
                .findFirst();
        System.out.printf("  %s%n", firstWithE.orElse("未找到"));

        System.out.printf("%n【任务3】是否所有水果都包含字母'a'？%n");
        boolean allContainA = fruits.stream()
                .allMatch(s -> s.contains("a"));
        System.out.printf("  结果：%s%n", allContainA ? "是" : "否");
    }
}
