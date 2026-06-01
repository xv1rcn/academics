public class DrawingApp {
    public static void main(String[] args) {
        Shape circle = Shape.creatShape("circle", 5.0);
        Shape rectangle = Shape.creatShape("rectangle", 4.0, 6.0);
        Shape triangle = Shape.creatShape("triangle", 3.0, 4.0, 5.0);

        Shape[] shapes = {circle, rectangle, triangle};
        
        for (Shape shape : shapes) {
            shape.draw();
            shape.printInfo();
        }
    }
}
