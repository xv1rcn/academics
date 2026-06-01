public abstract class MessageHandler {
    protected MessageHandler nextHandler;

    public MessageHandler setNext(MessageHandler handler) {
        this.nextHandler = handler;
        return handler; // 链式
    }

    public abstract void handle(Message msg);

    protected void doNext(Message msg) {
        if (nextHandler != null) {
            nextHandler.handle(msg);
        }
    }
}
