public class Cow extends Animal implements Mammal {
    @Override
    public void makeSound() {
        System.out.println("哞哞");
    }

    @Override
    public String getType() {
        return "牛";
    }

    @Override
    public void feedMilk() {
        System.out.println("小牛在喝母乳...");
    }
}
