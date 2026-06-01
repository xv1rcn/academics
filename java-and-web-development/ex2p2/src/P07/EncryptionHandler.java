public class EncryptionHandler extends MessageHandler {
    @Override
    public void handle(Message msg) {
        String original = msg.getContent();
        String reversed = new StringBuilder(original).reverse().toString();
        msg.setContent(reversed);
        doNext(msg);
    }
}
