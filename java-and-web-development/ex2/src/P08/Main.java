public class Main {
    public static void main(String[] args) {
        Employee e1 = new Employee("张三", "研发部", 8000);
        Employee e2 = new Employee("李四", "人事部", 6500);
        Employee e3 = new Employee("王五", "财务部", 7200);

        System.out.println("当前员工总人数：" + Employee.getTotalEmployees());
    }
}
