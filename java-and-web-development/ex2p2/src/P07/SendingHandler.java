public class SendingHandler extends MessageHandler {
    @Override
    public void handle(Message msg) {
        System.out.println("发送消息: " + msg.getContent());
    }
}
