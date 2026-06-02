public class AccountTest {
    public static void main(String[] args) throws InterruptedException {
        Account account = new Account(1000, "ACC001");

        Thread t1 = new Thread(new Runnable() {
            @Override
            public void run() {
                account.withdraw(800);
            }
        });

        Thread t2 = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                account.withdraw(1000);
            }
        });

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.printf("最终余额：%d%n", account.getBalance());
    }
}
