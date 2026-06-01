import java.io.FileNotFoundException;

class Child1 extends Parent {
    @Override
    public void readFile() throws FileNotFoundException {
        System.out.printf("子类Child1.readFile() - 重写后仅抛出FileNotFoundException%n");
    }
}
