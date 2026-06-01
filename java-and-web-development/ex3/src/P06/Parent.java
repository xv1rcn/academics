import java.io.IOException;

class Parent {
    public void readFile() throws IOException {
        System.out.printf("父类Parent.readFile() - 读取文件，可能抛出IOException%n");
    }
}
