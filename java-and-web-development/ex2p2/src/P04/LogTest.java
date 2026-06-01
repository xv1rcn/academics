public class LogTest {
    public static void main(String[] args) {
        Logger logger = Logger.builder()
        .appender(new ConsoleAppender())
        .level(LogLevel.WARN);
        logger.info("系统启动");
        logger.warn("磁盘空间不足");
        logger.error("数据库连接失败");

        logger.setAppender(new FileAppender("res/P04_FileAppender.log"));
        logger.level(LogLevel.INFO);
        logger.info("用户登录");
        logger.error("支付失败");

        logger.setAppender(new NullAppender());
        logger.info("这条日志无输出");
    }
}
