import java.util.HashMap;
import java.util.Map;

public class MapTraverse {
    @Override
    public String toString() {
        return "MapTraverse遍历演示";
    }

    public static void main(String[] args) {
        HashMap<String, String> map = new HashMap<>();
        map.put("北京", "Beijing");
        map.put("上海", "Shanghai");
        map.put("广州", "Guangzhou");
        map.put("深圳", "Shenzhen");
        map.put("杭州", "Hangzhou");
        map.put("成都", "Chengdu");
        map.put("武汉", "Wuhan");
        map.put("南京", "Nanjing");

        System.out.printf("=== HashMap遍历方式对比 ===%n%n");

        long start, end;

        start = System.nanoTime();
        for (String key : map.keySet()) {
            String value = map.get(key);
        }
        end = System.nanoTime();
        System.out.printf("1. keySet + get 遍历结果:%n");
        for (String key : map.keySet()) {
            System.out.printf("  %s -> %s%n", key, map.get(key));
        }
        System.out.printf("  耗时: %.4f ms%n%n", (end - start) / 1_000_000.0);

        start = System.nanoTime();
        for (Map.Entry<String, String> entry : map.entrySet()) {
            String key = entry.getKey();
            String value = entry.getValue();
        }
        end = System.nanoTime();
        System.out.printf("2. entrySet 遍历结果:%n");
        for (Map.Entry<String, String> entry : map.entrySet()) {
            System.out.printf("  %s -> %s%n", entry.getKey(), entry.getValue());
        }
        System.out.printf("  耗时: %.4f ms%n%n", (end - start) / 1_000_000.0);

        start = System.nanoTime();
        for (String value : map.values()) {
            String v = value;
        }
        end = System.nanoTime();
        System.out.printf("3. values 遍历结果(仅值):%n");
        for (String value : map.values()) {
            System.out.printf("  %s%n", value);
        }
        System.out.printf("  耗时: %.4f ms%n%n", (end - start) / 1_000_000.0);

        start = System.nanoTime();
        map.forEach((k, v) -> {
            String key = k;
            String value = v;
        });
        end = System.nanoTime();
        System.out.printf("4. Lambda forEach 遍历结果:%n");
        map.forEach((k, v) -> System.out.printf("  %s -> %s%n", k, v));
        System.out.printf("  耗时: %.4f ms%n%n", (end - start) / 1_000_000.0);

        System.out.printf("=== 大规模遍历性能对比(50万次) ===%n%n");

        start = System.nanoTime();
        for (int i = 0; i < 500_000; i++) {
            for (String key : map.keySet()) {
                String value = map.get(key);
            }
        }
        end = System.nanoTime();
        double t1 = (end - start) / 1_000_000.0;
        System.out.printf("1. keySet + get:   %.2f ms%n", t1);

        start = System.nanoTime();
        for (int i = 0; i < 500_000; i++) {
            for (Map.Entry<String, String> entry : map.entrySet()) {
                String key = entry.getKey();
                String value = entry.getValue();
            }
        }
        end = System.nanoTime();
        double t2 = (end - start) / 1_000_000.0;
        System.out.printf("2. entrySet:       %.2f ms%n", t2);

        start = System.nanoTime();
        for (int i = 0; i < 500_000; i++) {
            for (String value : map.values()) {
                String v = value;
            }
        }
        end = System.nanoTime();
        double t3 = (end - start) / 1_000_000.0;
        System.out.printf("3. values:         %.2f ms%n", t3);

        start = System.nanoTime();
        for (int i = 0; i < 500_000; i++) {
            map.forEach((k, v) -> {
                String key = k;
                String value = v;
            });
        }
        end = System.nanoTime();
        double t4 = (end - start) / 1_000_000.0;
        System.out.printf("4. Lambda forEach: %.2f ms%n", t4);

        System.out.printf("%n=== 效率分析 ===%n");
        System.out.printf("entrySet 遍历效率最高,直接访问内部Entry对象,避免额外hash查找。%n");
        System.out.printf("keySet+get 效率最低,因为每次get()都需重新计算hashCode并查找桶位。%n");
        System.out.printf("values 遍历仅访问值集合,结构最简单,仅需值时优先使用。%n");
        System.out.printf("Lambda forEach 语法简洁,底层基于entrySet实现,性能与entrySet接近。%n");
        System.out.printf("推荐:需要键值对时使用entrySet,仅需值时使用values,追求简洁使用Lambda forEach。%n");
    }
}
