public class Main {
    public static void main(String[] args) {
        Complex c1 = new Complex(3.2, 2.6);
        Complex c2 = new Complex(3.2, -1.5);
        Complex c3 = new Complex(0, 0.5);
        Complex c4 = new Complex(1.5, 0);

        System.out.println("c1 = " + c1);
        System.out.println("c2 = " + c2);
        System.out.println("c3 = " + c3);
        System.out.println("c4 = " + c4);

        System.out.println("c1 + c2 = " + c1.add(c2));
        System.out.println("c1 - c2 = " + c1.sub(c2));
        System.out.println("c1 * c2 = " + c1.mul(c2));
        System.out.println("c1 / c2 = " + c1.div(c2));
    }
}
