public class Triangle extends Shape {
    private double a;
    private double b;
    private double c;

    public Triangle(double a, double b, double c) {
        if (!isValidTriangle(a, b, c)) {
            throw new IllegalArgumentException("三边无法构成三角形!");
        }
        this.a = a;
        this.b = b;
        this.c = c;
    }

    private static boolean isValidTriangle(double a, double b, double c) {
        return a > 0 && b > 0 && c > 0 && (a + b > c) && (a + c > b) && (b + c > a);
    }

    @Override
    public double area() {
        double s = (a + b + c) / 2;
        return Math.sqrt(s * (s - a) * (s - b) * (s - c));
    }

    @Override
    public double perimeter() {
        return a + b + c;
    }

    @Override
    public void draw() {
        System.out.println("绘制三角形, 三边: " + a + ", " + b + ", " + c);
    }
}
