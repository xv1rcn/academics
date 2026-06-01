public class InsufficientBalanceException extends RuntimeException {
    public InsufficientBalanceException(double balance, double amount) {
        super(String.format("余额不足！当前余额: %.2f, 取款金额: %.2f", balance, amount));
    }
}
