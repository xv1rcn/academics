public class Dog extends Animal implements Mammal {
    @Override
    public void makeSound() {
        System.out.println("汪汪");
    }

    @Override
    public String getType() {
        return "狗";
    }

    @Override
    public void feedMilk() {
        System.out.println("小狗在喝母乳...");
    }
}
