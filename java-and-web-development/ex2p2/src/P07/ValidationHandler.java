public class ValidationHandler extends MessageHandler {
    @Override
    public void handle(Message msg) {
        if (msg.getContent() == null || msg.getContent().isEmpty()) {
            throw new IllegalArgumentException("消息内容不能为空");
        }
        doNext(msg);
    }
}
