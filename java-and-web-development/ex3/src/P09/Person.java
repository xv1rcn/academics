public class Person {
    private String name;
    private int age;

    public Person(String name, int age) {
        if (name == null || name.isEmpty()) {
            throw new IllegalArgumentException("姓名不能为空");
        }
        if (age < 0 || age > 150) {
            throw new IllegalArgumentException("年龄无效：" + age);
        }
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    @Override
    public String toString() {
        return String.format("Person{姓名='%s', 年龄=%d}", name, age);
    }
}
