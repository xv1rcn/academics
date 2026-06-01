public class ConstructorTest {
    public static void main(String[] args) {
        System.out.printf("=== P09 构造方法测试 ===%n%n");

        System.out.printf("--- 合法对象测试 ---%n");
        createAndPrint("张三", 25);
        createAndPrint("李四", 0);
        createAndPrint("王五", 150);

        System.out.printf("%n--- 非法对象测试 ---%n");
        createAndPrint(null, 0);
        createAndPrint("", 0);
        createAndPrint("赵六", -5);
        createAndPrint("赵六", 200);
    }

    private static void createAndPrint(String name, int age) {
        try {
            Person p = new Person(name, age);
            System.out.printf("创建成功: %s%n", p);
        } catch (IllegalArgumentException e) {
            System.out.printf("创建失败: %s%n", e.getMessage());
        }
    }
}
