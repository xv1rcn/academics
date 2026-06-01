import java.io.IOException;

public class OverrideTest {
    public static void main(String[] args) {
        Parent[] parents = {new Parent(), new Child1(), new Child2()};
        System.out.printf("========== 多态数组遍历：父类引用调用readFile() ==========%n");
        for (int i = 0; i < parents.length; i++) {
            System.out.printf("--- 第%d个对象: %s ---%n", i + 1, parents[i].getClass().getSimpleName());
            try {
                parents[i].readFile();
                System.out.printf("调用成功，无异常发生%n");
            } catch (IOException e) {
                System.out.printf("捕获异常: %s [%s]%n", e.getClass().getSimpleName(), e.getMessage());
            }
            System.out.printf("%n");
        }
    }
}
