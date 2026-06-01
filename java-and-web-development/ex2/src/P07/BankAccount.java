public class BankAccount {
    private String accountNumber;
    private double balance;

    private static double annualInterestRate;

    public BankAccount(String accountNumber) {
        this.accountNumber = accountNumber;
        this.balance = 0.0;
    }

    public void deposit(double amount) {
        if (amount <= 0) {
            System.out.println("存款金额必须大于 0");
        } else {
            balance += amount;
        }
    }

    public void withdraw(double amount) {
        if (amount <= 0) {
            System.out.println("取款金额必须大于 0");
        } else if (amount > balance) {
            System.out.println("余额不足，取款失败");
        } else {
            balance -= amount;
        }
    }

    public double getMonthlyInterest() {
        return balance * (annualInterestRate / 12);
    }

    public static void setInterestRate(double rate) {
        if (rate < 0) {
            System.out.println("利率必须大于或等于 0");
        } else {
            annualInterestRate = rate;
        }
    }

    public String getAccountNumber() {
        return accountNumber;
    }

    public double getBalance() {
        return balance;
    }
}
