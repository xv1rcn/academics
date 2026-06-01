public class Main {
    public static void main(String[] args) {
        BankAccount.setInterestRate(0.12);


        BankAccount acc1 = new BankAccount("12345678");
        BankAccount acc2 = new BankAccount("12345679");

        acc1.deposit(5000);
        acc1.withdraw(1000);

        acc2.deposit(10000);
        acc2.withdraw(12000);

        System.out.println(
            "账户 " + acc1.getAccountNumber() +
            " 的余额: " + acc1.getBalance() +
            ", 每月利息: " + acc1.getMonthlyInterest()
        );
        System.out.println(
            "账户 " + acc2.getAccountNumber() +
            " 的余额: " + acc2.getBalance() +
            ", 每月利息: " + acc2.getMonthlyInterest()
        );
    }
}
