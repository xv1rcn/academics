import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

public class WordCount {
    public static void main(String[] args) {
        String text = "hello world hello java world java programming";
        String[] words = text.toLowerCase().split(" ");
        HashMap<String, Integer> map = new HashMap<>();
        for (String word : words) {
            map.put(word, map.getOrDefault(word, 0) + 1);
        }
        TreeMap<String, Integer> sorted = new TreeMap<>(map);
        for (Map.Entry<String, Integer> entry : sorted.entrySet()) {
            System.out.printf("%s: %d%n", entry.getKey(), entry.getValue());
        }
        System.out.printf("不重复单词数: %d%n", map.size());
    }
}
