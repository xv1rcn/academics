public class ThreadCreation {
    public static void main(String[] args) {
        Thread t1 = new ExtendThread();
        Thread t2 = new Thread(new ImplementRunnable(), "实现线程");
        t1.start();
        t2.start();
    }
}
