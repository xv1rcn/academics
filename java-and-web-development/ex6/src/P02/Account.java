public class Account {
    private int balance;
    private String accountNo;

    public Account(int balance, String accountNo) {
        this.balance = balance;
        this.accountNo = accountNo;
    }

    public synchronized void withdraw(double amount) {
        if (amount > balance) {
            System.out.printf("账户%s 余额不足%n", accountNo);
        } else {
            balance -= amount;
            System.out.printf("账户%s 取款成功，当前余额：%.0f%n", accountNo, (double) balance);
        }
    }

    public int getBalance() {
        return balance;
    }
}
