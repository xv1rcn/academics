import java.util.List;

public class StudentDaoTest {
    public static void main(String[] args) {
        StudentDAOImpl dao = new StudentDAOImpl();

        System.out.printf("========== DAO模式测试 ==========%n");

        System.out.printf("%n--- 插入2个学生 ---%n");
        Student s1 = new Student(1, "张三", 20);
        Student s2 = new Student(2, "李四", 22);
        int r1 = dao.insert(s1);
        int r2 = dao.insert(s2);
        System.out.printf("插入结果: %s -> %d 行, %s -> %d 行%n", s1.getName(), r1, s2.getName(), r2);

        System.out.printf("%n--- 查询全部学生 ---%n");
        List<Student> all = dao.findAll();
        System.out.printf("共 %d 条记录:%n", all.size());
        for (Student s : all) {
            System.out.printf("  %s%n", s);
        }

        System.out.printf("%n--- 修改张三的年龄为25 ---%n");
        s1.setAge(25);
        int r3 = dao.update(s1);
        System.out.printf("更新结果: %d 行%n", r3);

        System.out.printf("%n--- 按ID=1查询验证 ---%n");
        Student found = dao.findById(1);
        if (found != null) {
            System.out.printf("查询结果: %s%n", found);
        } else {
            System.out.printf("未找到ID=1的学生%n");
        }

        System.out.printf("%n--- 删除ID=2的学生(李四) ---%n");
        int r4 = dao.delete(2);
        System.out.printf("删除结果: %d 行%n", r4);

        System.out.printf("%n--- 删除后查询全部确认 ---%n");
        List<Student> all2 = dao.findAll();
        System.out.printf("共 %d 条记录:%n", all2.size());
        for (Student s : all2) {
            System.out.printf("  %s%n", s);
        }

        System.out.printf("%n========== 测试完成 ==========%n");

        SimpleConnectionPool.shutdown();
    }
}
