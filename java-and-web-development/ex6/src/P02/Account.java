public class Account {
    private int balance;
    private String accountNo;

    public Account(int balance, String accountNo) {
        this.balance = balance;
        this.accountNo = accountNo;
    }

    public synchronized void withdraw(double amount) {
        if (amount > balance) {
            System.out.printf("余额不足%n");
        } else {
            balance -= amount;
            System.out.printf("取款成功，当前余额：%.0f%n", (double) balance);
        }
    }

    public int getBalance() {
        return balance;
    }
}
