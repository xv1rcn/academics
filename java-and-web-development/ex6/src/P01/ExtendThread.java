public class ExtendThread extends Thread {
    public ExtendThread() {
        super("继承线程");
    }

    @Override
    public void run() {
        for (int i = 1; i <= 5; i++) {
            System.out.printf("%s 第%d次执行%n", Thread.currentThread().getName(), i);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
