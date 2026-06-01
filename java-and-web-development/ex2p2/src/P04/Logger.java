import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Logger {
    private LogAppender appender;
    private LogLevel threshold;

    private Logger() {}

    public static Logger builder() {
        return new Logger();
    }

    public Logger appender(LogAppender appender) {
        this.appender = appender;
        return this; // 链式
    }

    public Logger level(LogLevel threshold) {
        this.threshold = threshold;
        return this; // 链式
    }

    public void setAppender(LogAppender appender) {
        this.appender = appender;
    }

    public void log(LogLevel level, String message) {
        if (level.ordinal() < threshold.ordinal()) {
            return;
        }
        String formattedMsg = formatMessage(level, message);
        appender.append(formattedMsg);
    }

    private String formatMessage(LogLevel level, String message) {
        String time = LocalDateTime.now().format(
            DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")
        );
        return String.format("[%s] [%s] %s", time, level, message);
    }

    public void info(String msg) { log(LogLevel.INFO, msg); }
    public void warn(String msg) { log(LogLevel.WARN, msg); }
    public void error(String msg) { log(LogLevel.ERROR, msg); }
}
