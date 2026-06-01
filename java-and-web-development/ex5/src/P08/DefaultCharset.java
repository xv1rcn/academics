import java.nio.charset.Charset;

public class DefaultCharset {
    public static void main(String[] args) {
        System.out.printf("本平台字符流默认字符集: %s%n", Charset.defaultCharset().name());
    }
}
