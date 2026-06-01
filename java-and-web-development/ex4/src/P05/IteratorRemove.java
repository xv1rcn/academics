import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class IteratorRemove {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<>();
        for (int i = 1; i <= 10; i++) {
            list.add(i);
        }

        System.out.printf("原始列表: %s%n", list);

        Iterator<Integer> it = list.iterator();
        while (it.hasNext()) {
            Integer num = it.next();
            if (num % 2 == 0) {
                it.remove();
            }
        }

        System.out.printf("删除偶数后: %s%n", list);
    }
}
