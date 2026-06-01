import java.util.ArrayList;
import java.util.List;

public class AnimalSimulator {
    public static void main(String[] args) {
        List<Animal> animals = new ArrayList<>();
        animals.add(Animal.createByName("Dog"));
        animals.add(Animal.createByName("Cat"));
        animals.add(Animal.createByName("Cow"));
        animals.add(Animal.createByName("Sparrow"));

        for (Animal animal : animals) {
            System.out.println("动物类型: " + animal.getType());
            animal.eat();
            animal.makeSound();

            if (animal instanceof Mammal) {
                ((Mammal) animal).feedMilk();
            } else if (animal instanceof Bird) {
                ((Bird) animal).fly();
            }

            System.out.println();
        }
    }
}
