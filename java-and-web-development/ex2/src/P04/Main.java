public class Main {
    public static void main(String[] args) {
        Rectangle r1 = new Rectangle(5, 3);
        System.out.println("矩形的宽度: " + r1.getWidth());
        System.out.println("矩形的高度: " + r1.getHeight());
        System.out.println("矩形的面积: " + r1.getArea());
        System.out.println("矩形的周长: " + r1.getPerimeter());

        try {
            Rectangle r2 = new Rectangle(-4, -6);
        } catch (IllegalArgumentException e) {
            System.out.println("错误: " + e.getMessage());
        }
    }
}
