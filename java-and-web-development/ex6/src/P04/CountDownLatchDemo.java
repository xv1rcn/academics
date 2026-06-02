import java.util.concurrent.CountDownLatch;

public class CountDownLatchDemo {
    public static void main(String[] args) throws InterruptedException {
        CountDownLatch latch = new CountDownLatch(3);
        for (int i = 1; i <= 3; i++) {
            final int id = i;
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Thread.sleep((long) (Math.random() * 2000 + 1000));
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.printf("线程%d准备就绪%n", id);
                    latch.countDown();
                }
            }).start();
        }
        latch.await();
        System.out.printf("所有准备工作完成，主线程开始执行%n");
    }
}
