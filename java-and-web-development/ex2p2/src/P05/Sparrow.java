public class Sparrow extends Animal implements Bird {
    @Override
    public void makeSound() {
        System.out.println("叽叽");
    }

    @Override
    public String getType() {
        return "麻雀";
    }

    @Override
    public void fly() {
        System.out.println("麻雀在飞...");
    }
}
