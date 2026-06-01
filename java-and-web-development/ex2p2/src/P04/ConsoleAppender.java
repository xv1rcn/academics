public class ConsoleAppender implements LogAppender{
    @Override
    public void append(String message) {
        System.out.println(message);
    }
}
