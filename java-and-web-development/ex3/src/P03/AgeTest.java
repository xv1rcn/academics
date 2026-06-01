public class AgeTest {
    public static void main(String[] args) {
        Person person = new Person();

        try {
            person.setAge(-5);
        } catch (InvalidAgeException e) {
            System.out.printf("捕获异常: %s%n", e.getMessage());
        }

        try {
            person.setAge(25);
            System.out.printf("年龄设置成功: %d%n", person.getAge());
        } catch (InvalidAgeException e) {
            System.out.printf("捕获异常: %s%n", e.getMessage());
        }
    }
}
