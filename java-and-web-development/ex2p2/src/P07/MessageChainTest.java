public class MessageChainTest {
    public static void main(String[] args) {
        System.out.println("=== 正常消息处理 ===");
        MessageHandler chain = new ValidationHandler();
        chain.setNext(new LoggingHandler())
             .setNext(new EncryptionHandler())
             .setNext(new SendingHandler());

        Message msg = new Message("你好，责任链模式！");
        chain.handle(msg);

        System.out.println("\n=== 空消息处理（应抛出异常） ===");
        try {
            Message emptyMsg = new Message("");
            chain.handle(emptyMsg);
        } catch (IllegalArgumentException e) {
            System.out.println("捕获异常: " + e.getMessage());
        }

        System.out.println("\n=== null 消息处理（应抛出异常） ===");
        try {
            Message nullMsg = new Message(null);
            chain.handle(nullMsg);
        } catch (IllegalArgumentException e) {
            System.out.println("捕获异常: " + e.getMessage());
        }
    }
}
