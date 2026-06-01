public class Person {
    private int age;

    public void setAge(int age) throws InvalidAgeException {
        if (age < 0 || age > 150) {
            throw new InvalidAgeException("年龄必须在0~150之间");
        }
        this.age = age;
    }

    public int getAge() {
        return age;
    }
}
