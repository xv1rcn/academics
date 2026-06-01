public class Main {
    public static void main(String[] args) {
        Shape[] shapes = new Shape[2];
        shapes[0] = new Circle(5);
        shapes[1] = new Rectangle(4, 6);

        for (Shape shape : shapes) {
            System.out.println(shape.getClass().getCanonicalName() + " 面积: " + shape.getArea());
        }
    }
}
