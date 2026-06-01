public class BankTest {
    public static void main(String[] args) {
        BankAccount account = new BankAccount(1000);

        try {
            account.withdraw(1200);
        } catch (InsufficientBalanceException e) {
            System.out.printf("异常捕获: %s%n", e.getMessage());
        }

        account.withdraw(500);
        System.out.printf("取款成功，当前余额: %.2f%n", account.getBalance());
    }
}
