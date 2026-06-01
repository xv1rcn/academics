import java.util.Set;
import java.util.HashSet;
import java.util.Arrays;

public class SetOperations {
    public static void main(String[] args) {
        Set<Integer> setA = new HashSet<>(Arrays.asList(1, 2, 3, 4, 5));
        Set<Integer> setB = new HashSet<>(Arrays.asList(4, 5, 6, 7, 8));

        System.out.printf("原集合 A: %s%n", setA);
        System.out.printf("原集合 B: %s%n%n", setB);

        Set<Integer> union = new HashSet<>(setA);
        union.addAll(setB);
        System.out.printf("并集 (A ∪ B): %s%n", union);

        Set<Integer> intersection = new HashSet<>(setA);
        intersection.retainAll(setB);
        System.out.printf("交集 (A ∩ B): %s%n", intersection);

        Set<Integer> difference = new HashSet<>(setA);
        difference.removeAll(setB);
        System.out.printf("差集 (A - B): %s%n", difference);

        Set<Integer> diffAB = new HashSet<>(setA);
        diffAB.removeAll(setB);
        Set<Integer> diffBA = new HashSet<>(setB);
        diffBA.removeAll(setA);
        Set<Integer> symmetricDiff = new HashSet<>(diffAB);
        symmetricDiff.addAll(diffBA);
        System.out.printf("对称差 (A Δ B): %s%n%n", symmetricDiff);

        System.out.printf("验证原集合未变:%n");
        System.out.printf("原集合 A: %s%n", setA);
        System.out.printf("原集合 B: %s%n", setB);
    }
}
