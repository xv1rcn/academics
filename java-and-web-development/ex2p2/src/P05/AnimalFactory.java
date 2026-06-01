import java.util.HashMap;
import java.util.Map;
import java.util.function.Supplier;

public class AnimalFactory {
    private static final Map<String, Supplier<Animal>> registry = new HashMap<>();

    static {
        registerAnimal("Dog", Dog::new);
        registerAnimal("Cat", Cat::new);
        registerAnimal("Cow", Cow::new);
        registerAnimal("Sparrow", Sparrow::new);
    }

    public static void registerAnimal(String name, Supplier<Animal> creator) {
        registry.put(name.toLowerCase(), creator);
    }

    public static Animal createAnimal(String name) {
        Supplier<Animal> creator = registry.get(name.toLowerCase());
        if (creator == null) {
            throw new IllegalArgumentException("未知动物: " + name);
        }
        return creator.get();
    }
}
