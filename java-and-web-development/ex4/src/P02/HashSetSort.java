import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.ArrayList;

public class HashSetSort {
    public static void main(String[] args) {
        int[] arr = {5, 2, 8, 2, 5, 3, 1, 8, 9};
        HashSet<Integer> set = new HashSet<>();
        for (int num : arr) {
            set.add(num);
        }
        List<Integer> list = new ArrayList<>(set);
        System.out.printf("原始数组: %s%n", Arrays.toString(arr));
        System.out.printf("去重后集合: %s%n", set.toString());
        Collections.sort(list);
        System.out.printf("升序排序: %s%n", list.toString());
        Collections.sort(list, Collections.reverseOrder());
        System.out.printf("降序排序: %s%n", list.toString());
    }
}
