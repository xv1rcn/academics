public abstract class Animal {
    public abstract void makeSound();

    public abstract String getType();

    public void eat() {
        System.out.println("进食中...");
    }

    public static Animal createByName(String name) {
        return AnimalFactory.createAnimal(name);
    }
}
