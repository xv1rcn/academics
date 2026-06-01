import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;

public class ArrayToList {
    public static List<Integer> arrayToList(int[] arr) {
        List<Integer> list = new ArrayList<>();
        for (int i = 0; i < arr.length; i++) {
            list.add(arr[i]);
        }
        return list;
    }

    public static int[] listToArray(List<Integer> list) {
        int[] arr = new int[list.size()];
        for (int i = 0; i < list.size(); i++) {
            arr[i] = list.get(i);
        }
        return arr;
    }

    public static void main(String[] args) {
        System.out.printf("=== 数组与List互转测试 ===%n%n");

        System.out.printf("--- 测试1: 多元素数组 {10,20,30,40,50} ---%n");
        int[] arr1 = {10, 20, 30, 40, 50};
        System.out.printf("原始数组: %s%n", Arrays.toString(arr1));
        List<Integer> list1 = arrayToList(arr1);
        System.out.printf("转换为List: %s%n", list1);
        list1.add(60);
        System.out.printf("添加60后List: %s%n", list1);
        int[] result1 = listToArray(list1);
        System.out.printf("转回数组: %s%n", Arrays.toString(result1));

        System.out.printf("%n--- 测试2: 空数组 {} ---%n");
        int[] arr2 = {};
        System.out.printf("原始空数组: %s%n", Arrays.toString(arr2));
        List<Integer> list2 = arrayToList(arr2);
        System.out.printf("空数组转List: %s (size=%d)%n", list2, list2.size());
        int[] result2 = listToArray(list2);
        System.out.printf("空List转回数组: %s%n", Arrays.toString(result2));

        System.out.printf("%n--- 测试3: 单元素数组 {99} ---%n");
        int[] arr3 = {99};
        System.out.printf("原始数组: %s%n", Arrays.toString(arr3));
        List<Integer> list3 = arrayToList(arr3);
        System.out.printf("单元素转List: %s (size=%d)%n", list3, list3.size());
        int[] result3 = listToArray(list3);
        System.out.printf("单元素List转回数组: %s%n", Arrays.toString(result3));
    }
}
