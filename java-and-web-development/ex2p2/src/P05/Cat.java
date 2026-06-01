public class Cat extends Animal implements Mammal {
    @Override
    public void makeSound() {
        System.out.println("喵喵");
    }

    @Override
    public String getType() {
        return "猫";
    }

    @Override
    public void feedMilk() {
        System.out.println("小猫在喝母乳...");
    }
}
