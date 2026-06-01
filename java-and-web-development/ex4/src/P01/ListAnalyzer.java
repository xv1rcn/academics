import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class ListAnalyzer {
    public static void main(String[] args) {
        List<String> list = new ArrayList<>(Arrays.asList("apple", "banana", "apple", "orange", "grape", "banana", "apple"));

        System.out.printf("删除前列表: %s%n", list);

        Iterator<String> it = list.iterator();
        while (it.hasNext()) {
            String item = it.next();
            if ("banana".equals(item)) {
                it.remove();
            }
        }

        System.out.printf("删除后列表: %s%n", list);

        Map<String, Integer> freqMap = new HashMap<>();
        for (String item : list) {
            freqMap.put(item, freqMap.getOrDefault(item, 0) + 1);
        }

        System.out.printf("元素频次: %s%n", freqMap);
    }
}
