public class Main {
    public static void main(String[] args) {
        Animal dog = new Dog();
        Animal cat = new Cat();

        Zoo.makeSound(dog);
        Zoo.makeSound(cat);

        Animal animal = new Animal();
        Zoo.makeSound(animal);
    }
}
