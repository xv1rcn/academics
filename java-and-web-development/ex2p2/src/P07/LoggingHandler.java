import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class LoggingHandler extends MessageHandler {
    @Override
    public void handle(Message msg) {
        String time = LocalDateTime.now().format(
            DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")
        );
        System.out.println("[" + time + "] " + msg.getContent());
        doNext(msg);
    }
}
